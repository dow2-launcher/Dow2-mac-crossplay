// scatfix.c - crossplay_fix v26, part 32: WeaponUtil::GetEntitiesInScatterArea's angle test sums the dot product as DOW2.exe.
// Why (difftest step 11, angelgate 2025-12-13 Low F1335 / High H1130): RangedWeapon::GetPossibleBurstTargets (Mac ret 0x101c77da9 /
// DOW2.exe 0x9abfe1) collects the burst targets with GetEntitiesInScatterArea (Mac 0x101c98a30 / DOW2.exe 0x98e370). For each
// candidate: d = pos - origin, r = 1 / sqrt(|d|^2), c = dot(d * r, dir) clamped to [-1, 1], |acos(c)| > halfangle -> reject.
// All inputs and the three products are bit-identical; the sum is not:
//   Mac 0x101c9922b   c = (y + x) + z        DOW2.exe 0x98e857/0x98e85b   c = (z + y) + x       (MSVC /fp:fast order)
// Near c = 1 one ulp of c is ~270 ulp of the angle: in f1130 the Mac accepted candidate 16493 (angle 3d742ad2 <= 3d742bc9) and
// Windows rejected it (3d742bde) -> another burst group. t26 (1M cases): 21 % of c and 17 % of the angles differ in the port.
// The acos itself already is Windows' ((float)ms_acos((double)c) = msvcr80 __libm_sse2_acos + cvtsd2ss): mathfix hooks _acosf.
// What: 16 bytes rewritten in place, same length, no stub (xmm3 is dead after, 0x101c9923b movaps xmm3, xmm2; xmm4 untouched):
//   old  addss xmm3,xmm4 | mulss xmm0,[rbp-0x18e4] | addss xmm0,xmm3        ((y + x) + z)
//   new  mulss xmm0,[rbp-0x18e4] | addss xmm0,xmm3 | addss xmm0,xmm4        ((z + y) + x)
// Self-test at load: SC_NST cases from DOW2.exe (t26/w26 scatst: its own bytes 0x98e7ac.. run) through the PATCHED code in place
// (entered at 0x101c99181 with a fake frame in rbp and a fake entity in r13, temporary landing jump on the _acosf call 0x101c9925e
// returning c). DOW2_NOSCATFIX=1: off.
#include "patch_util.h"
#include "scatfix_st.inc"
#define SC_AT 0x101c9922bUL
#define SC_LAND 0x101c9925eUL
#define SC_ENTRY 0x101c99181UL
static const unsigned char SC_OLD[16] = { 0xf3,0x0f,0x58,0xdc, 0xf3,0x0f,0x59,0x85,0x1c,0xe7,0xff,0xff, 0xf3,0x0f,0x58,0xc3 };
static const unsigned char SC_NEW[16] = { 0xf3,0x0f,0x59,0x85,0x1c,0xe7,0xff,0xff, 0xf3,0x0f,0x58,0xc3, 0xf3,0x0f,0x58,0xc4 };
static const unsigned char SC_LAND_OLD[14] = { 0xe8,0x2d,0x86,0xc9,0xfe, 0xe8,0xd8,0x7d,0xc9,0xfe, 0x0f,0x2e,0x85,0x14 };
uint32_t sc_out; uint64_t sc_saved_rsp;
void sc_run(char *rbpv, uint64_t entry, char *r13v);
void sc_land(void);
__asm__(".text\n.globl _sc_run\n_sc_run:\n push %rbp\n push %rbx\n push %r12\n push %r13\n push %r14\n push %r15\n"
        " mov %rsp, _sc_saved_rsp(%rip)\n sub $0x108, %rsp\n mov %rdi, %rbp\n mov %rdx, %r13\n jmp *%rsi\n"
        ".globl _sc_land\n_sc_land:\n movss %xmm0, _sc_out(%rip)\n"
        " mov _sc_saved_rsp(%rip), %rsp\n pop %r15\n pop %r14\n pop %r13\n pop %r12\n pop %rbx\n pop %rbp\n ret\n");
static int sc_wr(unsigned char *p, const void *b, size_t n) {
    vm_address_t page = (vm_address_t)p & ~(vm_address_t)(vm_page_size - 1);
    if (vm_protect(mach_task_self(), page, vm_page_size * 2, 0, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY) != KERN_SUCCESS) return 0;
    memcpy(p, b, n); vm_protect(mach_task_self(), page, vm_page_size * 2, 0, VM_PROT_READ | VM_PROT_EXECUTE); return 1;
}
static char sc_fr[0x2000], sc_ent[0x200];
static uint32_t sc_case(const uint32_t *c, uintptr_t slide) {   // pos [r13+0xc0..]; origin -0x1900 -0x1910 -0x18e8; dir -0x18e0 -0x1920 -0x18e4
    char *rb = sc_fr + 0x1f00; memcpy(sc_ent + 0xc0, c, 12);
    memcpy(rb - 0x1900, c + 3, 4); memcpy(rb - 0x1910, c + 4, 4); memcpy(rb - 0x18e8, c + 5, 4);
    memcpy(rb - 0x18e0, c + 6, 4); memcpy(rb - 0x1920, c + 7, 4); memcpy(rb - 0x18e4, c + 8, 4);
    sc_out = 0xdeadbeef; sc_run(rb, SC_ENTRY + slide, sc_ent); return sc_out;
}
static unsigned long sc_selftest(uintptr_t slide, int *ok) {
    unsigned char *l = (unsigned char *)(SC_LAND + slide); unsigned long bad = 0; *ok = 0;
    if (memcmp(l, SC_LAND_OLD, 14) || !pu_jump(l, (void *)sc_land)) return 0;
    for (unsigned long q = 0; q < SC_NST; q++) if (sc_case(SC_ST[q], slide) != SC_ST[q][9]) bad++;
    if (!sc_wr(l, SC_LAND_OLD, 14)) return bad + 1;
    *ok = 1; return bad;
}
__attribute__((constructor)) static void scatfix_init(void) {
    if (getenv("DOW2_NOSCATFIX")) { pu_log("scatfix: DOW2_NOSCATFIX set - the port's own scatter-area dot product order (y + x) + z%.0lu%.0lu", 0, 0); return; }
    uintptr_t slide = (uintptr_t)_dyld_get_image_vmaddr_slide(0);
    unsigned char *p = (unsigned char *)(SC_AT + slide);
    if (memcmp(p, SC_OLD, sizeof SC_OLD)) { pu_log("scatfix: unexpected bytes at %#lx (another game build?) - NOT patched%.0lu", SC_AT, 0); return; }
    if (!sc_wr(p, SC_NEW, sizeof SC_NEW)) { pu_log("scatfix: vm_protect failed - NOT patched%.0lu%.0lu", 0, 0); return; }
    int ok; unsigned long bad = sc_selftest(slide, &ok);
    if (!ok || bad) { sc_wr(p, SC_OLD, sizeof SC_OLD);
        pu_log(ok ? "scatfix self-test: %lu of %lu differ from DOW2.exe - NOT patched" : "scatfix self-test could not run (%lu/%lu) - NOT patched", bad, SC_NST); return; }
    pu_log("scatfix: OK - WeaponUtil::GetEntitiesInScatterArea dot product (z + y) + x as DOW2.exe, self-test %lu (patched code run)%.0lu", SC_NST, 0);
}
