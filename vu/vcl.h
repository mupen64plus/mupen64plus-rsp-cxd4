/*
 * mupen64plus-rsp-cxd4 - RSP Interpreter
 * Copyright (C) 2012-2013  RJ 'Iconoclast' Swedlow
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "vu.h"

INLINE static void do_cl(short* VD, short* VS, short* VT)
{
    short eq[N], ge[N], le[N];
    short gen[N], len[N], lz[N], uz[N], sn[N];
    short diff[N];
    short cmp[N];
    unsigned short VB[N], VC[N];
    register int i;

    for (i = 0; i < N; i++)
        VB[i] = VS[i];
    for (i = 0; i < N; i++)
        VC[i] = VT[i];

/*
    for (i = 0; i < N; i++)
        ge[i] = clip[i];
    for (i = 0; i < N; i++)
        le[i] = comp[i];
*/
    for (i = 0; i < N; i++)
        eq[i] = ne[i] ^ 1;
    for (i = 0; i < N; i++)
        sn[i] = co[i];
/*
 * Now that we have extracted all the flags, we will essentially be masking
 * them back in where they came from redundantly, unless the corresponding
 * NOTEQUAL bit from VCO upper was not set....
 */
    for (i = 0; i < N; i++)
        VC[i] = VC[i] ^ -sn[i];
    for (i = 0; i < N; i++)
        VC[i] = VC[i] + sn[i]; /* conditional negation, if sn */
    for (i = 0; i < N; i++)
        diff[i] = VB[i] - VC[i];
    for (i = 0; i < N; i++)
        uz[i] = (VB[i] - VC[i] - 0xFFFF) >> 31;
    for (i = 0; i < N; i++)
        lz[i] = (diff[i] == 0x0000);
    for (i = 0; i < N; i++)
        gen[i] = lz[i] | uz[i];
    for (i = 0; i < N; i++)
        len[i] = lz[i] & uz[i];
    for (i = 0; i < N; i++)
        gen[i] = gen[i] & vce[i];
    for (i = 0; i < N; i++)
        len[i] = len[i] & (vce[i] ^ 1);
    for (i = 0; i < N; i++)
        len[i] = len[i] | gen[i];
    for (i = 0; i < N; i++)
        gen[i] = (VB[i] >= VC[i]);

    for (i = 0; i < N; i++)
        cmp[i] = eq[i] & sn[i];
    merge(le, cmp, len, comp);

    for (i = 0; i < N; i++)
        cmp[i] = eq[i] & (sn[i] ^ 1);
    merge(ge, cmp, gen, clip);

    merge(cmp, sn, le, ge);
    merge(VACC_L, cmp, (short *)VC, VS);
    vector_copy(VD, VACC_L);

    for (i = 0; i < N; i++)
        clip[i] = ge[i];
    for (i = 0; i < N; i++)
        comp[i] = le[i];
    for (i = 0; i < N; i++)
        ne[i] = 0;
    for (i = 0; i < N; i++)
        co[i] = 0;
    for (i = 0; i < N; i++)
        vce[i] = 0;
    return;
}

static void VCL(void)
{
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;

    do_cl(VR[vd], VR[vs], ST);
    return;
}

static void VCL_v(void)
{
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    do_cl(VR[vd], VR[vs], VR[vt]);
    return;
}
static void VCL0q(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x2);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL1q(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x3);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL0h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x4);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL1h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x5);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL2h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x6);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL3h(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x7);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL0w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x8);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL1w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0x9);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL2w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xA);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL3w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xB);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL4w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xC);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL5w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xD);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL6w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xE);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
static void VCL7w(void)
{
    short SV[N];
    const int vd = inst.R.sa;
    const int vs = inst.R.rd;
    const int vt = inst.R.rt;

    SHUFFLE_VECTOR(SV, VR[vt], 0xF);
    do_cl(VR[vd], VR[vs], SV);
    return;
}
