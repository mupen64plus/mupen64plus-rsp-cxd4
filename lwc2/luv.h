/******************************************************************************\
* Project:  SP VU Emulation Table:  Load Packed Unsigned to Vector Unit (LUV)  *
* Creator:  R. J. Swedlow                                                      *
* Release:  2012.12.24                                                         *
* License:  none (public domain)                                               *
\******************************************************************************/

void LUV(int vt, int element, signed offset, int base)
{
    register unsigned int addr = SR[base] + (offset * 8);

    addr &= 0x00000FFF;
    if (element || addr & 0x007)
    { /* World Driver Championship gfx ucode will do this in intro demo. */
        register int i;

        message("LUV\nCrossed doubleword index barrier.", 0);
        element = -element;
        for (i = 0; i < 8; i++)
        {
            VR[vt].s[i] = RSP.DMEM[(addr + (element++ & 0xF)) ^ 03] << 7;
        }
        return;
    }
    VR[vt].s[07] = RSP.DMEM[addr + (07 ^ 03)] << 7;
    VR[vt].s[06] = RSP.DMEM[addr + (06 ^ 03)] << 7;
    VR[vt].s[05] = RSP.DMEM[addr + (05 ^ 03)] << 7;
    VR[vt].s[04] = RSP.DMEM[addr + (04 ^ 03)] << 7;
    VR[vt].s[03] = RSP.DMEM[addr + (03 ^ 03)] << 7;
    VR[vt].s[02] = RSP.DMEM[addr + (02 ^ 03)] << 7;
    VR[vt].s[01] = RSP.DMEM[addr + (01 ^ 03)] << 7;
    VR[vt].s[00] = RSP.DMEM[addr + (00 ^ 03)] << 7;
/* Official documentation shows that there are exactly eight executions.
 * Although they must also occur simultaneously in the vector unit, the most
 * significant element (VR[vt].s[07]) is listed first in the operation
 * definition, whose sequence therefore must be:  `for (i = 7; i != 0; --i)`.
 */
    return;
}