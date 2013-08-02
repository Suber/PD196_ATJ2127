/*
* Automatically generated register definition: don't edit
* GL5110 Spec Version_V1.09
* Sat 12-30-2011  16:39:56
*/
#ifndef __GL5110_REG_DEFINITION_H___
#define __GL5110_REG_DEFINITION_H___


//--------------MemoryController-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     MemoryController_BASE                                             0xc00a0000
#define     MemoryCtl                                                         (MemoryController_BASE+0x00000000)
#define     AlternateInstr0                                                   (MemoryController_BASE+0x00000004)
#define     AlternateInstr1                                                   (MemoryController_BASE+0x00000008)
#define     AlternateInstr2                                                   (MemoryController_BASE+0x0000000c)
#define     AlternateInstr3                                                   (MemoryController_BASE+0x00000010)
#define     AlternateInstr4                                                   (MemoryController_BASE+0x00000014)
#define     AlternateInstr5                                                   (MemoryController_BASE+0x00000018)
#define     AlternateInstr6                                                   (MemoryController_BASE+0x0000001c)
#define     FixAddr0                                                          (MemoryController_BASE+0x00000020)
#define     FixAddr1                                                          (MemoryController_BASE+0x00000024)
#define     FixAddr2                                                          (MemoryController_BASE+0x00000028)
#define     FixAddr3                                                          (MemoryController_BASE+0x0000002c)
#define     FixAddr4                                                          (MemoryController_BASE+0x00000030)
#define     FixAddr5                                                          (MemoryController_BASE+0x00000034)
#define     FixAddr6                                                          (MemoryController_BASE+0x00000038)
#define     bist_en                                                           (MemoryController_BASE+0x0000003c)
#define     bist_fin                                                          (MemoryController_BASE+0x00000040)
#define     bist_info                                                         (MemoryController_BASE+0x00000044)
#define     PageMissCtl                                                       (MemoryController_BASE+0x00000048)
#define     PageAddr0                                                         (MemoryController_BASE+0x0000004c)
#define     PageAddr1                                                         (MemoryController_BASE+0x00000050)
#define     PageAddr2                                                         (MemoryController_BASE+0x00000054)
#define     PageAddr3                                                         (MemoryController_BASE+0x00000058)
#define     PageAddr4                                                         (MemoryController_BASE+0x0000005c)
#define     PageAddr5                                                         (MemoryController_BASE+0x00000060)
#define     PageAddr6                                                         (MemoryController_BASE+0x00000064)
#define     PageAddr7                                                         (MemoryController_BASE+0x00000068)
#define     PageAddr8                                                         (MemoryController_BASE+0x0000006c)
#define     PageAddr9                                                         (MemoryController_BASE+0x00000070)
#define     PageAddr10                                                        (MemoryController_BASE+0x00000074)
#define     PageAddr11                                                        (MemoryController_BASE+0x00000078)
#define     PageAddr12                                                        (MemoryController_BASE+0x0000007c)
#define     PageAddr13                                                        (MemoryController_BASE+0x00000080)
#define     PageAddr14                                                        (MemoryController_BASE+0x00000084)
#define     PageAddr15                                                        (MemoryController_BASE+0x00000088)
#define     PageAddr16                                                        (MemoryController_BASE+0x0000008c)
#define     PageAddr17                                                        (MemoryController_BASE+0x00000090)
#define     PageAddr18                                                        (MemoryController_BASE+0x00000094)
#define     PageAddr19                                                        (MemoryController_BASE+0x00000098)
#define     PageMissInstr                                                     (MemoryController_BASE+0x0000009c)

//--------------Bits Location------------------------------------------//
#define     MemoryCtl_jtag_disable                                            9
#define     MemoryCtl_Spi_boot_disable                                        8
#define     MemoryCtl_FixEn6                                                  6
#define     MemoryCtl_FixEn5                                                  5
#define     MemoryCtl_FixEn4                                                  4
#define     MemoryCtl_FixEn3                                                  3
#define     MemoryCtl_FixEn2                                                  2
#define     MemoryCtl_FixEn1                                                  1
#define     MemoryCtl_FixEn0                                                  0

#define     AlternateInstr0_AlternatInstr0_e                                  31
#define     AlternateInstr0_AlternatInstr0_SHIFT                              0
#define     AlternateInstr0_AlternatInstr0_MASK                               (0xFFFFFFFF<<0)

#define     AlternateInstr1_AlternatInstr1_e                                  31
#define     AlternateInstr1_AlternatInstr1_SHIFT                              0
#define     AlternateInstr1_AlternatInstr1_MASK                               (0xFFFFFFFF<<0)

#define     AlternateInstr2_AlternatInstr2_e                                  31
#define     AlternateInstr2_AlternatInstr2_SHIFT                              0
#define     AlternateInstr2_AlternatInstr2_MASK                               (0xFFFFFFFF<<0)

#define     AlternateInstr3_AlternatInstr3_e                                  31
#define     AlternateInstr3_AlternatInstr3_SHIFT                              0
#define     AlternateInstr3_AlternatInstr3_MASK                               (0xFFFFFFFF<<0)

#define     AlternateInstr4_AlternatInstr4_e                                  31
#define     AlternateInstr4_AlternatInstr4_SHIFT                              0
#define     AlternateInstr4_AlternatInstr4_MASK                               (0xFFFFFFFF<<0)

#define     AlternateInstr5_AlternatInstr5_e                                  31
#define     AlternateInstr5_AlternatInstr5_SHIFT                              0
#define     AlternateInstr5_AlternatInstr5_MASK                               (0xFFFFFFFF<<0)

#define     AlternateInstr6_AlternatInstr6_e                                  31
#define     AlternateInstr6_AlternatInstr6_SHIFT                              0
#define     AlternateInstr6_AlternatInstr6_MASK                               (0xFFFFFFFF<<0)

#define     FixAddr0_FixAddr0_e                                               17
#define     FixAddr0_FixAddr0_SHIFT                                           2
#define     FixAddr0_FixAddr0_MASK                                            (0xFFFF<<2)

#define     FixAddr1_FixAddr1_e                                               17
#define     FixAddr1_FixAddr1_SHIFT                                           2
#define     FixAddr1_FixAddr1_MASK                                            (0xFFFF<<2)

#define     FixAddr2_FixAddr2_e                                               17
#define     FixAddr2_FixAddr2_SHIFT                                           2
#define     FixAddr2_FixAddr2_MASK                                            (0xFFFF<<2)

#define     FixAddr3_FixAddr3_e                                               17
#define     FixAddr3_FixAddr3_SHIFT                                           2
#define     FixAddr3_FixAddr3_MASK                                            (0xFFFF<<2)

#define     FixAddr4_FixAddr4_e                                               17
#define     FixAddr4_FixAddr4_SHIFT                                           2
#define     FixAddr4_FixAddr4_MASK                                            (0xFFFF<<2)

#define     FixAddr5_FixAddr5_e                                               17
#define     FixAddr5_FixAddr5_SHIFT                                           2
#define     FixAddr5_FixAddr5_MASK                                            (0xFFFF<<2)

#define     FixAddr6_FixAddr6_e                                               17
#define     FixAddr6_FixAddr6_SHIFT                                           2
#define     FixAddr6_FixAddr6_MASK                                            (0xFFFF<<2)

#define     bist_en_ADCRAM_bist_en                                            24
#define     bist_en_DACRAM_bist_en                                            23
#define     bist_en_MUROM3_bist_en                                            22
#define     bist_en_MUROM2_bist_en                                            21
#define     bist_en_MUROM1_bist_en                                            20
#define     bist_en_FLASHRAM01_bist_en                                        19
#define     bist_en_JRAM3_bist_en                                             18
#define     bist_en_JRAM2_bist_en                                             17
#define     bist_en_URAM_bist_en                                              16
#define     bist_en_PCMRAM_bist_en                                            15
#define     bist_en_MURAM3_bist_en                                            14
#define     bist_en_MURAM1_bist_en                                            13
#define     bist_en_MURAM2_bist_en                                            12
#define     bist_en_JRAM1_bist_en                                             11
#define     bist_en_JRAM0_bist_en                                             10
#define     bist_en_JRAM5_bist_en                                             9
#define     bist_en_RAM8_bist_en                                              8
#define     bist_en_RAM7_bist_en                                              7
#define     bist_en_RAM6_bist_en                                              6
#define     bist_en_RAM5_bist_en                                              5
#define     bist_en_RAM4_bist_en                                              4
#define     bist_en_RAM3_bist_en                                              3
#define     bist_en_RAM2_bist_en                                              2
#define     bist_en_RAM1_bist_en                                              1
#define     bist_en_RAM0_bist_en                                              0

#define     bist_fin_ADCRAM_bist_fin                                          24
#define     bist_fin_DACRAM_bist_fin                                          23
#define     bist_fin_MUROM3_bist_fin                                          22
#define     bist_fin_MUROM2_bist_fin                                          21
#define     bist_fin_MUROM1_bist_fin                                          20
#define     bist_fin_FLASHRAM01_bist_fin                                      19
#define     bist_fin_JRAM3_bist_fin                                           18
#define     bist_fin_JRAM2_bist_fin                                           17
#define     bist_fin_URAM_bist_fin                                            16
#define     bist_fin_PCMRAM_bist_fin                                          15
#define     bist_fin_MURAM3_bist_fin                                          14
#define     bist_fin_MURAM1_bist_fin                                          13
#define     bist_fin_MURAM2_bist_fin                                          12
#define     bist_fin_JRAM1_bist_fin                                           11
#define     bist_fin_JRAM0_bist_fin                                           10
#define     bist_fin_JRAM5_bist_fin                                           9
#define     bist_fin_RAM8_bist_fin                                            8
#define     bist_fin_RAM7_bist_fin                                            7
#define     bist_fin_RAM6_bist_fin                                            6
#define     bist_fin_RAM5_bist_fin                                            5
#define     bist_fin_RAM4_bist_fin                                            4
#define     bist_fin_RAM3_bist_fin                                            3
#define     bist_fin_RAM2_bist_fin                                            2
#define     bist_fin_RAM1_bist_fin                                            1
#define     bist_fin_RAM0_bist_fin                                            0

#define     bist_info_ADCRAM_bist_info                                        24
#define     bist_info_DACRAM_bist_info                                        23
#define     bist_info_MUROM3_bist_info                                        22
#define     bist_info_MUROM2_bist_info                                        21
#define     bist_info_MUROM1_bist_info                                        20
#define     bist_info_FLASHRAM01_bist_info                                    19
#define     bist_info_JRAM3_bist_info                                         18
#define     bist_info_JRAM2_bist_info                                         17
#define     bist_info_URAM_bist_info                                          16
#define     bist_info_PCMRAM_bist_info                                        15
#define     bist_info_MURAM3_bist_info                                        14
#define     bist_info_MURAM1_bist_info                                        13
#define     bist_info_MURAM2_bist_info                                        12
#define     bist_info_JRAM1_bist_info                                         11
#define     bist_info_JRAM0_bist_info                                         10
#define     bist_info_JRAM5_bist_info                                         9
#define     bist_info_RAM8_bist_info                                          8
#define     bist_info_RAM7_bist_info                                          7
#define     bist_info_RAM6_bist_info                                          6
#define     bist_info_RAM5_bist_info                                          5
#define     bist_info_RAM4_bist_info                                          4
#define     bist_info_RAM3_bist_info                                          3
#define     bist_info_RAM2_bist_info                                          2
#define     bist_info_RAM1_bist_info                                          1
#define     bist_info_RAM0_bist_info                                          0

#define     PageMissCtl_IS_ERROR_EN                                           0

#define     PageAddr0_PagAddr_e                                               31
#define     PageAddr0_PagAddr_SHIFT                                           18
#define     PageAddr0_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr1_PagAddr_e                                               31
#define     PageAddr1_PagAddr_SHIFT                                           18
#define     PageAddr1_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr2_PagAddr_e                                               31
#define     PageAddr2_PagAddr_SHIFT                                           18
#define     PageAddr2_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr3_PagAddr_e                                               31
#define     PageAddr3_PagAddr_SHIFT                                           18
#define     PageAddr3_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr4_PagAddr_e                                               31
#define     PageAddr4_PagAddr_SHIFT                                           18
#define     PageAddr4_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr5_PagAddr_e                                               31
#define     PageAddr5_PagAddr_SHIFT                                           18
#define     PageAddr5_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr6_PagAddr_e                                               31
#define     PageAddr6_PagAddr_SHIFT                                           18
#define     PageAddr6_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr7_PagAddr_e                                               31
#define     PageAddr7_PagAddr_SHIFT                                           18
#define     PageAddr7_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr8_PagAddr_e                                               31
#define     PageAddr8_PagAddr_SHIFT                                           18
#define     PageAddr8_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr9_PagAddr_e                                               31
#define     PageAddr9_PagAddr_SHIFT                                           18
#define     PageAddr9_PagAddr_MASK                                            (0x3FFF<<18)

#define     PageAddr10_PagAddr_e                                              31
#define     PageAddr10_PagAddr_SHIFT                                          18
#define     PageAddr10_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr11_PagAddr_e                                              31
#define     PageAddr11_PagAddr_SHIFT                                          18
#define     PageAddr11_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr12_PagAddr_e                                              31
#define     PageAddr12_PagAddr_SHIFT                                          18
#define     PageAddr12_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr13_PagAddr_e                                              31
#define     PageAddr13_PagAddr_SHIFT                                          18
#define     PageAddr13_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr14_PagAddr_e                                              31
#define     PageAddr14_PagAddr_SHIFT                                          18
#define     PageAddr14_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr15_PagAddr_e                                              31
#define     PageAddr15_PagAddr_SHIFT                                          18
#define     PageAddr15_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr16_PagAddr_e                                              31
#define     PageAddr16_PagAddr_SHIFT                                          18
#define     PageAddr16_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr17_PagAddr_e                                              31
#define     PageAddr17_PagAddr_SHIFT                                          18
#define     PageAddr17_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr18_PagAddr_e                                              31
#define     PageAddr18_PagAddr_SHIFT                                          18
#define     PageAddr18_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageAddr19_PagAddr_e                                              31
#define     PageAddr19_PagAddr_SHIFT                                          18
#define     PageAddr19_PagAddr_MASK                                           (0x3FFF<<18)

#define     PageMissInstr_Opcode_e                                            31
#define     PageMissInstr_Opcode_SHIFT                                        0
#define     PageMissInstr_Opcode_MASK                                         (0xFFFFFFFF<<0)

//--------------PMU-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     PMU_BASE                                                          0xc0020000
#define     VOUT_CTL                                                          (PMU_BASE+0x00)
#define     VDD_DCDC_CTL                                                      (PMU_BASE+0x04)
#define     CHG_CTL                                                           (PMU_BASE+0x18)
#define     CHG_DET                                                           (PMU_BASE+0x1c)
#define     CHG_ASSISTANT                                                     (PMU_BASE+0x20)
#define     DIODE_CTL                                                         (PMU_BASE+0x24)
#define     BDG_CTL                                                           (PMU_BASE+0x28)
#define     USB_EFUSE_REF                                                     (PMU_BASE+0x2c)
#define     PMUADC_CTL                                                        (PMU_BASE+0x30)
#define     BATADC_DATA                                                       (PMU_BASE+0x34)
#define     LRADC2_DATA                                                       (PMU_BASE+0x38)
#define     LRADC1_DATA                                                       (PMU_BASE+0x3c)
#define     LRADC3_DATA                                                       (PMU_BASE+0x40)
#define     LRADC4_DATA                                                       (PMU_BASE+0x44)
#define     LRADC5_DATA                                                       (PMU_BASE+0x48)
#define     MULTI_USED                                                        (PMU_BASE+0x4c)
#define     SYSTEM_VOL                                                        (PMU_BASE+0x50)
#define     SYSTEM_CTL_RTCVDD                                                 (PMU_BASE+0x54)
#define     PRESS_DISPLAY                                                     (PMU_BASE+0x58)

//--------------Bits Location------------------------------------------//
#define     VOUT_CTL_SD_DISCHG                                                8
#define     VOUT_CTL_VCC_LDO_I                                                7
#define     VOUT_CTL_VCC_SET_e                                                6
#define     VOUT_CTL_VCC_SET_SHIFT                                            4
#define     VOUT_CTL_VCC_SET_MASK                                             (0x7<<4)
#define     VOUT_CTL_VDD_SET_e                                                3
#define     VOUT_CTL_VDD_SET_SHIFT                                            0
#define     VOUT_CTL_VDD_SET_MASK                                             (0xF<<0)

#define     VDD_DCDC_CTL_EN_FAST                                              12
#define     VDD_DCDC_CTL_PWMPFM_CTL                                           11
#define     VDD_DCDC_CTL_VDD_ZERO_CTL_e                                       10
#define     VDD_DCDC_CTL_VDD_ZERO_CTL_SHIFT                                   8
#define     VDD_DCDC_CTL_VDD_ZERO_CTL_MASK                                    (0x7<<8)
#define     VDD_DCDC_CTL_VDD_AUTO                                             7
#define     VDD_DCDC_CTL_VDD_MODU                                             6
#define     VDD_DCDC_CTL_DCDC_MAX_I_e                                         5
#define     VDD_DCDC_CTL_DCDC_MAX_I_SHIFT                                     4
#define     VDD_DCDC_CTL_DCDC_MAX_I_MASK                                      (0x3<<4)
#define     VDD_DCDC_CTL_DCDC_FS_e                                            3
#define     VDD_DCDC_CTL_DCDC_FS_SHIFT                                        1
#define     VDD_DCDC_CTL_DCDC_FS_MASK                                         (0x7<<1)
#define     VDD_DCDC_CTL_PFM_FS                                               0

#define     CHG_CTL_CHG_POWER                                                 7
#define     CHG_CTL_CHG_EN                                                    5
#define     CHG_CTL_TRICKLE_EN                                                4
#define     CHG_CTL_CHG_CURRENT_e                                             3
#define     CHG_CTL_CHG_CURRENT_SHIFT                                         0
#define     CHG_CTL_CHG_CURRENT_MASK                                          (0xF<<0)

#define     CHG_DET_TEMP_STATUS                                               7
#define     CHG_DET_CHG_TEST                                                  6
#define     CHG_DET_CHG_PHASE_e                                               5
#define     CHG_DET_CHG_PHASE_SHIFT                                           4
#define     CHG_DET_CHG_PHASE_MASK                                            (0x3<<4)
#define     CHG_DET_CHG_I_DET_e                                               3
#define     CHG_DET_CHG_I_DET_SHIFT                                           1
#define     CHG_DET_CHG_I_DET_MASK                                            (0x7<<1)
#define     CHG_DET_CHG_STATUS                                                0

#define     CHG_ASSISTANT_CHG_TEM_e                                           5
#define     CHG_ASSISTANT_CHG_TEM_SHIFT                                       4
#define     CHG_ASSISTANT_CHG_TEM_MASK                                        (0x3<<4)
#define     CHG_ASSISTANT_CHG_VOL                                             0

#define     DIODE_CTL_DIODE1_EN                                               2
#define     DIODE_CTL_EN_FASTOFF                                              1
#define     DIODE_CTL_EN_FASTON                                               0

#define     BDG_CTL_BDG_SEL                                                   8
#define     BDG_CTL_BDG_PD                                                    7
#define     BDG_CTL_BDG_EN                                                    6
#define     BDG_CTL_Reserved_A                                                5
#define     BDG_CTL_BDG_FILTER                                                4
#define     BDG_CTL_BDG_VOL_e                                                 3
#define     BDG_CTL_BDG_VOL_SHIFT                                             0
#define     BDG_CTL_BDG_VOL_MASK                                              (0xF<<0)

#define     USB_EFUSE_REF_Efacen                                              7
#define     USB_EFUSE_REF_R_SEL                                               6
#define     USB_EFUSE_REF_UREF_e                                              5
#define     USB_EFUSE_REF_UREF_SHIFT                                          0
#define     USB_EFUSE_REF_UREF_MASK                                           (0x3F<<0)

#define     PMUADC_CTL_ADC_FS                                                 7
#define     PMUADC_CTL_BATADC_EN                                              6
#define     PMUADC_CTL_LRADC1_EN                                              5
#define     PMUADC_CTL_LRADC1_IRQ_EN                                          4
#define     PMUADC_CTL_LRADC1_IRQ_VOL                                         3
#define     PMUADC_CTL_LRADC1_PENDING                                         2

#define     BATADC_DATA_BATADC_e                                              6
#define     BATADC_DATA_BATADC_SHIFT                                          0
#define     BATADC_DATA_BATADC_MASK                                           (0x7F<<0)

#define     LRADC2_DATA_LRADC2_e                                              6
#define     LRADC2_DATA_LRADC2_SHIFT                                          0
#define     LRADC2_DATA_LRADC2_MASK                                           (0x7F<<0)

#define     LRADC1_DATA_LRADC1_e                                              5
#define     LRADC1_DATA_LRADC1_SHIFT                                          0
#define     LRADC1_DATA_LRADC1_MASK                                           (0x3F<<0)

#define     LRADC3_DATA_LRADC3_e                                              5
#define     LRADC3_DATA_LRADC3_SHIFT                                          0
#define     LRADC3_DATA_LRADC3_MASK                                           (0x3F<<0)

#define     LRADC4_DATA_LRADC4_e                                              5
#define     LRADC4_DATA_LRADC4_SHIFT                                          0
#define     LRADC4_DATA_LRADC4_MASK                                           (0x3F<<0)

#define     LRADC5_DATA_LRADC5_e                                              5
#define     LRADC5_DATA_LRADC5_SHIFT                                          0
#define     LRADC5_DATA_LRADC5_MASK                                           (0x3F<<0)

#define     MULTI_USED_SEG_DISP_VCC_EN                                        31
#define     MULTI_USED_SDVCCOUT_UVLO                                          16
#define     MULTI_USED_SDVCCOUT_EN                                            15
#define     MULTI_USED_SDVCCOUT_SST_e                                         14
#define     MULTI_USED_SDVCCOUT_SST_SHIFT                                     13
#define     MULTI_USED_SDVCCOUT_SST_MASK                                      (0x3<<13)
#define     MULTI_USED_SDVCCOUT_CURLIM_SET_e                                  12
#define     MULTI_USED_SDVCCOUT_CURLIM_SET_SHIFT                              11
#define     MULTI_USED_SDVCCOUT_CURLIM_SET_MASK                               (0x3<<11)
#define     MULTI_USED_SDVCCOUT_CURLIM_EN                                     10
#define     MULTI_USED_SDVCCOUT_VOL_e                                         9
#define     MULTI_USED_SDVCCOUT_VOL_SHIFT                                     8
#define     MULTI_USED_SDVCCOUT_VOL_MASK                                      (0x3<<8)
#define     MULTI_USED_UVDD_EN                                                7
#define     MULTI_USED_UVDD_V_e                                               6
#define     MULTI_USED_UVDD_V_SHIFT                                           4
#define     MULTI_USED_UVDD_V_MASK                                            (0x7<<4)
#define     MULTI_USED_USBVDD_PD                                              3
#define     MULTI_USED_USBVDD_PD2                                             2
#define     MULTI_USED_USBVDD_PD3                                             1
#define     MULTI_USED_FM_VCCOUT_EN                                           0

#define     SYSTEM_VOL_Reserved0_e                                            31
#define     SYSTEM_VOL_Reserved0_SHIFT                                        8
#define     SYSTEM_VOL_Reserved0_MASK                                         (0xFFFFFF<<8)
#define     SYSTEM_VOL_UVLO                                                   7
#define     SYSTEM_VOL_UVLO_DBC                                               6
#define     SYSTEM_VOL_UVLO_DET_EN                                            5
#define     SYSTEM_VOL_AVCC_DROP_e                                            2
#define     SYSTEM_VOL_AVCC_DROP_SHIFT                                        1
#define     SYSTEM_VOL_AVCC_DROP_MASK                                         (0x3<<1)
#define     SYSTEM_VOL_AVCC_BIASEN                                            0

#define     SYSTEM_CTL_RTCVDD_ONOFF_STATUS                                    31
#define     SYSTEM_CTL_RTCVDD_ONOFF_RESET_EN                                  12
#define     SYSTEM_CTL_RTCVDD_ONOFF_RESET_TIME_e                              11
#define     SYSTEM_CTL_RTCVDD_ONOFF_RESET_TIME_SHIFT                          10
#define     SYSTEM_CTL_RTCVDD_ONOFF_RESET_TIME_MASK                           (0x3<<10)
#define     SYSTEM_CTL_RTCVDD_VCC_SS_SET_e                                    9
#define     SYSTEM_CTL_RTCVDD_VCC_SS_SET_SHIFT                                8
#define     SYSTEM_CTL_RTCVDD_VCC_SS_SET_MASK                                 (0x3<<8)
#define     SYSTEM_CTL_RTCVDD_SYSON_TIME_e                                    7
#define     SYSTEM_CTL_RTCVDD_SYSON_TIME_SHIFT                                6
#define     SYSTEM_CTL_RTCVDD_SYSON_TIME_MASK                                 (0x3<<6)
#define     SYSTEM_CTL_RTCVDD_LB_EN                                           5
#define     SYSTEM_CTL_RTCVDD_LB_VOL_e                                        4
#define     SYSTEM_CTL_RTCVDD_LB_VOL_SHIFT                                    3
#define     SYSTEM_CTL_RTCVDD_LB_VOL_MASK                                     (0x3<<3)
#define     SYSTEM_CTL_RTCVDD_OC_EN                                           2
#define     SYSTEM_CTL_RTCVDD_LVPRO_EN                                        1
#define     SYSTEM_CTL_RTCVDD_REG_ENPMU                                       0

#define     PRESS_DISPLAY_SHORT_PRESS                                         7
#define     PRESS_DISPLAY_LONG_PRESS                                          6
#define     PRESS_DISPLAY_BIAS_CTL_e                                          5
#define     PRESS_DISPLAY_BIAS_CTL_SHIFT                                      4
#define     PRESS_DISPLAY_BIAS_CTL_MASK                                       (0x3<<4)
#define     PRESS_DISPLAY_PRESS_TEST                                          3
#define     PRESS_DISPLAY_TEST_STATUS                                         2

//--------------EFUSE-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     EFUSE_BASE                                                        0xc0010000
#define     EFUSE_CTL0                                                        (EFUSE_BASE+0x08)
#define     EFUSE_CTL1                                                        (EFUSE_BASE+0x0c)
#define     EFUSE_CTL2                                                        (EFUSE_BASE+0x10)
#define     EFUSE_DATA                                                        (EFUSE_BASE+0x14)

//--------------Bits Location------------------------------------------//
//#define     EFUSE_CTL0_CHIPID_CLKD                                            3£º1
#define     EFUSE_CTL0_DATA_READY                                             0

#define     EFUSE_CTL1_WR_PSD_OK                                              13
#define     EFUSE_CTL1_RD_PSD_OK                                              12
#define     EFUSE_CTL1_FS_PD                                                  11
#define     EFUSE_CTL1_EN_FS                                                  10
#define     EFUSE_CTL1_EN_VPP                                                 9
#define     EFUSE_CTL1_PROGRAM                                                8
#define     EFUSE_CTL1_ENB                                                    7
#define     EFUSE_CTL1_READ                                                   6
#define     EFUSE_CTL1_A5_A0_e                                                5
#define     EFUSE_CTL1_A5_A0_SHIFT                                            0
#define     EFUSE_CTL1_A5_A0_MASK                                             (0x3F<<0)

#define     EFUSE_CTL2_Password_e                                             31
#define     EFUSE_CTL2_Password_SHIFT                                         0
#define     EFUSE_CTL2_Password_MASK                                          (0xFFFFFFFF<<0)

#define     EFUSE_DATA_EFUSE_DATA_e                                           31
#define     EFUSE_DATA_EFUSE_DATA_SHIFT                                       0
#define     EFUSE_DATA_EFUSE_DATA_MASK                                        (0xFFFFFFFF<<0)

//--------------ChipVersion-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     ChipVersion_BASE                                                  0xc0000040
#define     ChipVersion                                                       (ChipVersion_BASE+0x00000000)

//--------------Bits Location------------------------------------------//
#define     ChipVersion_ChipVersion_e                                         3
#define     ChipVersion_ChipVersion_SHIFT                                     0
#define     ChipVersion_ChipVersion_MASK                                      (0xF<<0)

//--------------RMU_digital-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     RMU_digital_BASE                                                  0xc0000000
#define     MRCR                                                              (RMU_digital_BASE+0x00000000)

//--------------Bits Location------------------------------------------//
#define     MRCR_AESReset                                                     17
#define     MRCR_USBReset2                                                    16
#define     MRCR_BISTReset                                                    15
#define     MRCR_SEGLCDReset                                                  14
#define     MRCR_TouchKeyReset                                                13
#define     MRCR_PWMReset                                                     12
#define     MRCR_DACIISReset                                                  11
#define     MRCR_ADCReset                                                     10
#define     MRCR_USBReset                                                     9
#define     MRCR_NANDReset                                                    8
#define     MRCR_LCDReset                                                     7
#define     MRCR_SPIReset                                                     6
#define     MRCR_I2CReset                                                     5
#define     MRCR_UARTReset                                                    4
#define     MRCR_SDReset                                                      3
#define     MRCR_AudioCodecReset                                              2
#define     MRCR_VideoCodecReset                                              1
#define     MRCR_DMA0123Reset                                                 0

//--------------CMU_Analog_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     CMU_Analog_Register_BASE                                          0xC0000100
#define     MCUPLL_CTL                                                        (CMU_Analog_Register_BASE+0x00)
#define     HOSC_CTL                                                          (CMU_Analog_Register_BASE+0x04)
#define     LFPLL_CTL                                                         (CMU_Analog_Register_BASE+0x08)
#define     AUDIOPLL_CTL                                                      (CMU_Analog_Register_BASE+0x0C)
#define     MCUPLL_DEBUG                                                      (CMU_Analog_Register_BASE+0x10)
#define     LFPLL_DEBUG                                                       (CMU_Analog_Register_BASE+0x14)
#define     AUDIOPLL_DEBUG                                                    (CMU_Analog_Register_BASE+0x18)

//--------------Bits Location------------------------------------------//
#define     MCUPLL_CTL_LOCK                                                   7
#define     MCUPLL_CTL_MCUPLL_EN                                              6
#define     MCUPLL_CTL_SMCU_e                                                 5
#define     MCUPLL_CTL_SMCU_SHIFT                                             0
#define     MCUPLL_CTL_SMCU_MASK                                              (0x3F<<0)

#define     HOSC_CTL_V18VSEL_e                                                21
#define     HOSC_CTL_V18VSEL_SHIFT                                            20
#define     HOSC_CTL_V18VSEL_MASK                                             (0x3<<20)
#define     HOSC_CTL_V18VLDO_Enable                                           19
#define     HOSC_CTL_V18VLDO_PULLDN                                           18
#define     HOSC_CTL_V18VLDO_SS_Enable                                        17
#define     HOSC_CTL_V18VLDO_SS_CTRL                                          16
#define     HOSC_CTL_V12VSEL_e                                                13
#define     HOSC_CTL_V12VSEL_SHIFT                                            12
#define     HOSC_CTL_V12VSEL_MASK                                             (0x3<<12)
#define     HOSC_CTL_V12VLDO_Enable                                           11
#define     HOSC_CTL_V12VLDO_PULLDN                                           10
#define     HOSC_CTL_V12VLDO_SS_Enable                                        9
#define     HOSC_CTL_V12VLDO_SS_CTRL                                          8
#define     HOSC_CTL_HOSC_Limited_EN                                          7
#define     HOSC_CTL_HOSCCAPSEL_e                                             6
#define     HOSC_CTL_HOSCCAPSEL_SHIFT                                         4
#define     HOSC_CTL_HOSCCAPSEL_MASK                                          (0x7<<4)
#define     HOSC_CTL_HGMC_e                                                   3
#define     HOSC_CTL_HGMC_SHIFT                                               2
#define     HOSC_CTL_HGMC_MASK                                                (0x3<<2)
#define     HOSC_CTL_CK24M_SEL                                                1
#define     HOSC_CTL_HOSC_EN                                                  0

#define     LFPLL_CTL_LFPLL_OK2                                               9
#define     LFPLL_CTL_LFPLL_OK1                                               8
#define     LFPLL_CTL_LFPLL_LOCK                                              7
#define     LFPLL_CTL_SLFPLL_SEL                                              1
#define     LFPLL_CTL_LFPLL_EN                                                0

#define     AUDIOPLL_CTL_AUDPLL_LOCK                                          12
#define     AUDIOPLL_CTL_APS_e                                                11
#define     AUDIOPLL_CTL_APS_SHIFT                                            9
#define     AUDIOPLL_CTL_APS_MASK                                             (0x7<<9)
#define     AUDIOPLL_CTL_AUDPLLEN                                             8
#define     AUDIOPLL_CTL_ADCCLK_e                                             6
#define     AUDIOPLL_CTL_ADCCLK_SHIFT                                         4
#define     AUDIOPLL_CTL_ADCCLK_MASK                                          (0x7<<4)
#define     AUDIOPLL_CTL_DACCLK_e                                             2
#define     AUDIOPLL_CTL_DACCLK_SHIFT                                         0
#define     AUDIOPLL_CTL_DACCLK_MASK                                          (0x7<<0)

#define     MCUPLL_DEBUG_MCUPLL_PU_Force                                      31
#define     MCUPLL_DEBUG_MCUPLL_SET_forcevc_e                                 30
#define     MCUPLL_DEBUG_MCUPLL_SET_forcevc_SHIFT                             29
#define     MCUPLL_DEBUG_MCUPLL_SET_forcevc_MASK                              (0x3<<29)
#define     MCUPLL_DEBUG_IB_MCUPLL_e                                          28
#define     MCUPLL_DEBUG_IB_MCUPLL_SHIFT                                      27
#define     MCUPLL_DEBUG_IB_MCUPLL_MASK                                       (0x3<<27)
#define     MCUPLL_DEBUG_Reg_MCUPLL_CS_e                                      26
#define     MCUPLL_DEBUG_Reg_MCUPLL_CS_SHIFT                                  25
#define     MCUPLL_DEBUG_Reg_MCUPLL_CS_MASK                                   (0x3<<25)
#define     MCUPLL_DEBUG_Reg_MCUPLL_RS_e                                      24
#define     MCUPLL_DEBUG_Reg_MCUPLL_RS_SHIFT                                  23
#define     MCUPLL_DEBUG_Reg_MCUPLL_RS_MASK                                   (0x3<<23)
#define     MCUPLL_DEBUG_Reg_MCUPLL_ICHP_e                                    22
#define     MCUPLL_DEBUG_Reg_MCUPLL_ICHP_SHIFT                                21
#define     MCUPLL_DEBUG_Reg_MCUPLL_ICHP_MASK                                 (0x3<<21)
#define     MCUPLL_DEBUG_REG_MCUPLL_Fband_e                                   20
#define     MCUPLL_DEBUG_REG_MCUPLL_Fband_SHIFT                               19
#define     MCUPLL_DEBUG_REG_MCUPLL_Fband_MASK                                (0x3<<19)
#define     MCUPLL_DEBUG_TESTSEL_e                                            18
#define     MCUPLL_DEBUG_TESTSEL_SHIFT                                        17
#define     MCUPLL_DEBUG_TESTSEL_MASK                                         (0x3<<17)
#define     MCUPLL_DEBUG_TEST_EN                                              16
#define     MCUPLL_DEBUG_MCUPLL_Bypass                                        15
#define     MCUPLL_DEBUG_SSC_EN                                               14
#define     MCUPLL_DEBUG_SSC_DIV                                              13
#define     MCUPLL_DEBUG_SDM_ORDER                                            12
#define     MCUPLL_DEBUG_HNUM_e                                               11
#define     MCUPLL_DEBUG_HNUM_SHIFT                                           9
#define     MCUPLL_DEBUG_HNUM_MASK                                            (0x7<<9)
#define     MCUPLL_DEBUG_PSTEP_e                                              8
#define     MCUPLL_DEBUG_PSTEP_SHIFT                                          7
#define     MCUPLL_DEBUG_PSTEP_MASK                                           (0x3<<7)
#define     MCUPLL_DEBUG_FSTEP_e                                              6
#define     MCUPLL_DEBUG_FSTEP_SHIFT                                          1
#define     MCUPLL_DEBUG_FSTEP_MASK                                           (0x3F<<1)
#define     MCUPLL_DEBUG_FCODE_EXT_EN                                         0

#define     LFPLL_DEBUG_LFPLL_BYPASS                                          12
//#define     LFPLL_DEBUG_IB_LFPLL                                              11£º10
#define     LFPLL_DEBUG_LFPLL_CHP_Current_e                                   9
#define     LFPLL_DEBUG_LFPLL_CHP_Current_SHIFT                               6
#define     LFPLL_DEBUG_LFPLL_CHP_Current_MASK                                (0xF<<6)
#define     LFPLL_DEBUG_LFPLL_VCOBAND_e                                       5
#define     LFPLL_DEBUG_LFPLL_VCOBAND_SHIFT                                   4
#define     LFPLL_DEBUG_LFPLL_VCOBAND_MASK                                    (0x3<<4)
#define     LFPLL_DEBUG_LFPLL_MODE_e                                          3
#define     LFPLL_DEBUG_LFPLL_MODE_SHIFT                                      2
#define     LFPLL_DEBUG_LFPLL_MODE_MASK                                       (0x3<<2)
#define     LFPLL_DEBUG_LPF_control                                           1
#define     LFPLL_DEBUG_VCOSEL                                                0

#define     AUDIOPLL_DEBUG_AUDPLL_BYPASS                                      16
#define     AUDIOPLL_DEBUG_AUDPLL_PU_Force                                    15
#define     AUDIOPLL_DEBUG_AUDPLL_SET_forcevc_e                               14
#define     AUDIOPLL_DEBUG_AUDPLL_SET_forcevc_SHIFT                           13
#define     AUDIOPLL_DEBUG_AUDPLL_SET_forcevc_MASK                            (0x3<<13)
#define     AUDIOPLL_DEBUG_IB_AUDPLL_e                                        12
#define     AUDIOPLL_DEBUG_IB_AUDPLL_SHIFT                                    11
#define     AUDIOPLL_DEBUG_IB_AUDPLL_MASK                                     (0x3<<11)
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_CS_e                                    10
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_CS_SHIFT                                9
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_CS_MASK                                 (0x3<<9)
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_RS_e                                    8
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_RS_SHIFT                                7
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_RS_MASK                                 (0x3<<7)
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_ICHP_e                                  6
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_ICHP_SHIFT                              5
#define     AUDIOPLL_DEBUG_Reg_AUDPLL_ICHP_MASK                               (0x3<<5)
#define     AUDIOPLL_DEBUG_REG_AUDPLL_Fband_e                                 4
#define     AUDIOPLL_DEBUG_REG_AUDPLL_Fband_SHIFT                             3
#define     AUDIOPLL_DEBUG_REG_AUDPLL_Fband_MASK                              (0x3<<3)
#define     AUDIOPLL_DEBUG_TESTSEL_e                                          2
#define     AUDIOPLL_DEBUG_TESTSEL_SHIFT                                      1
#define     AUDIOPLL_DEBUG_TESTSEL_MASK                                       (0x3<<1)
#define     AUDIOPLL_DEBUG_TEST_EN                                            0

//--------------CMU_digital-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     CMU_digital_BASE                                                  0xC0000000
#define     CPUCLKCTL                                                         (CMU_digital_BASE+0x00000004)
#define     CLKENCTL                                                          (CMU_digital_BASE+0x00000008)
#define     VECLKCTL                                                          (CMU_digital_BASE+0x0000000c)
#define     AECLKCTL                                                          (CMU_digital_BASE+0x00000010)
#define     SDCLKCTL                                                          (CMU_digital_BASE+0x00000014)
#define     FMCLKCTL                                                          (CMU_digital_BASE+0x00000018)
#define     AESCLKCTL                                                         (CMU_digital_BASE+0x0000001c)
#define     LCDCLKCTL                                                         (CMU_digital_BASE+0x00000020)
#define     NANDCLKCTL                                                        (CMU_digital_BASE+0x00000024)
#define     PWMCLKCTL                                                         (CMU_digital_BASE+0x00000028)
#define     MEMCLKCTL0                                                        (CMU_digital_BASE+0x0000002c)
#define     MEMCLKCTL1                                                        (CMU_digital_BASE+0x00000030)

//--------------Bits Location------------------------------------------//
#define     CPUCLKCTL_AHBCLKDIV                                               4
#define     CPUCLKCTL_CPUCLKDIV_e                                             3
#define     CPUCLKCTL_CPUCLKDIV_SHIFT                                         2
#define     CPUCLKCTL_CPUCLKDIV_MASK                                          (0x3<<2)
#define     CPUCLKCTL_CPUCLKSEL_e                                             1
#define     CPUCLKCTL_CPUCLKSEL_SHIFT                                         0
#define     CPUCLKCTL_CPUCLKSEL_MASK                                          (0x3<<0)

#define     CLKENCTL_FM_CLKEN                                                 20
#define     CLKENCTL_AES_CLKEN                                                19
#define     CLKENCTL_SEGLCD_CLKEN                                             18
#define     CLKENCTL_TOUCHKEY_CLKEN                                           17
#define     CLKENCTL_PWMCLKEN                                                 16
#define     CLKENCTL_IR_CLKEN                                                 15
#define     CLKENCTL_I2S_INCLKEN                                              14
#define     CLKENCTL_DACCLKEN                                                 13
#define     CLKENCTL_ADCCLKEN                                                 12
#define     CLKENCTL_TIMER_PMUCLKEN                                           11
#define     CLKENCTL_USBCLKEN                                                 10
#define     CLKENCTL_NANDCLKEN                                                9
#define     CLKENCTL_LCDCLKEN                                                 8
#define     CLKENCTL_SPICLKEN                                                 7
#define     CLKENCTL_SPIBOOTCLKEN                                             6
#define     CLKENCTL_IICCLKEN                                                 5
#define     CLKENCTL_UARTCLKEN                                                4
#define     CLKENCTL_SDCLKEN                                                  3
#define     CLKENCTL_AECLKEN                                                  2
#define     CLKENCTL_VECLKEN                                                  1
#define     CLKENCTL_DMACLKEN                                                 0

#define     VECLKCTL_VECLKDIV_e                                               2
#define     VECLKCTL_VECLKDIV_SHIFT                                           1
#define     VECLKCTL_VECLKDIV_MASK                                            (0x3<<1)
#define     VECLKCTL_VECLKSEL                                                 0

#define     AECLKCTL_AECLKDIV0                                                3
#define     AECLKCTL_AECLKDIV1_e                                              2
#define     AECLKCTL_AECLKDIV1_SHIFT                                          1
#define     AECLKCTL_AECLKDIV1_MASK                                           (0x3<<1)
#define     AECLKCTL_AECLKSEL                                                 0

#define     SDCLKCTL_SDCLKSEL1                                                5
#define     SDCLKCTL_SDCLKDIV_e                                               4
#define     SDCLKCTL_SDCLKDIV_SHIFT                                           1
#define     SDCLKCTL_SDCLKDIV_MASK                                            (0xF<<1)
#define     SDCLKCTL_SDCLKSEL0                                                0

#define     FMCLKCTL_FMCLKSEL_e                                               1
#define     FMCLKCTL_FMCLKSEL_SHIFT                                           0
#define     FMCLKCTL_FMCLKSEL_MASK                                            (0x3<<0)

#define     AESCLKCTL_AESCLKDIV_e                                             2
#define     AESCLKCTL_AESCLKDIV_SHIFT                                         1
#define     AESCLKCTL_AESCLKDIV_MASK                                          (0x3<<1)
#define     AESCLKCTL_AESCLKSEL                                               0

#define     LCDCLKCTL_LCDCLKDIV_e                                             2
#define     LCDCLKCTL_LCDCLKDIV_SHIFT                                         1
#define     LCDCLKCTL_LCDCLKDIV_MASK                                          (0x3<<1)
#define     LCDCLKCTL_LCDCLKSEL                                               0

#define     NANDCLKCTL_DIV4CLKEN                                              3
#define     NANDCLKCTL_NANDCLKDIV0_e                                          2
#define     NANDCLKCTL_NANDCLKDIV0_SHIFT                                      1
#define     NANDCLKCTL_NANDCLKDIV0_MASK                                       (0x3<<1)
#define     NANDCLKCTL_NANDCLKSEL                                             0

#define     PWMCLKCTL_PWMCLKDIV_e                                             8
#define     PWMCLKCTL_PWMCLKDIV_SHIFT                                         1
#define     PWMCLKCTL_PWMCLKDIV_MASK                                          (0xFF<<1)
#define     PWMCLKCTL_PWMCLKSEL                                               0

#define     MEMCLKCTL0_BUF1CLKEN                                              25
#define     MEMCLKCTL0_BUF0CLKEN                                              24
#define     MEMCLKCTL0_URAMCLKEN                                              23
#define     MEMCLKCTL0_MURAM3CLKEN                                            22
#define     MEMCLKCTL0_MURAM2CLKEN                                            21
#define     MEMCLKCTL0_MURAM1CLKEN                                            20
#define     MEMCLKCTL0_PCMRAMCLKEN                                            19
#define     MEMCLKCTL0_JRAM5CLKEN                                             18
#define     MEMCLKCTL0_JRAM23CLKEN                                            17
#define     MEMCLKCTL0_JRAM1CLKEN                                             16
#define     MEMCLKCTL0_JRAM0CLKEN                                             15
#define     MEMCLKCTL0_RAM8CLKEN                                              14
#define     MEMCLKCTL0_RAM7CLKEN                                              13
#define     MEMCLKCTL0_RAM6CLKEN                                              12
#define     MEMCLKCTL0_RAM5CLKEN                                              11
#define     MEMCLKCTL0_RAM4CLKEN                                              10
#define     MEMCLKCTL0_RAM3CLKEN                                              9
#define     MEMCLKCTL0_RAM2CLKEN                                              8
#define     MEMCLKCTL0_RAM1CLKEN                                              7
#define     MEMCLKCTL0_RAM0CLKEN                                              6
#define     MEMCLKCTL0_ROM5CLKEN                                              5
#define     MEMCLKCTL0_ROM4CLKEN                                              4
#define     MEMCLKCTL0_ROM3CLKEN                                              3
#define     MEMCLKCTL0_ROM2CLKEN                                              2
#define     MEMCLKCTL0_ROM1CLKEN                                              1
#define     MEMCLKCTL0_ROM0CLKEN                                              0

#define     MEMCLKCTL1_BUF1CLKSEL                                             25
#define     MEMCLKCTL1_BUF0CLKSEL                                             24
#define     MEMCLKCTL1_URAMCLKSEL                                             23
#define     MEMCLKCTL1_MURAM3CLKSEL                                           22
#define     MEMCLKCTL1_MURAM1CLKSEL                                           21
#define     MEMCLKCTL1_PCMRAMCLKSEL                                           20
#define     MEMCLKCTL1_JRAM5CLKSEL_e                                          19
#define     MEMCLKCTL1_JRAM5CLKSEL_SHIFT                                      18
#define     MEMCLKCTL1_JRAM5CLKSEL_MASK                                       (0x3<<18)
#define     MEMCLKCTL1_JRAM23CLKSEL                                           17
#define     MEMCLKCTL1_JRAM1CLKSEL                                            16
#define     MEMCLKCTL1_JRAM0CLKSEL                                            15
#define     MEMCLKCTL1_RAM8CLKSEL                                             14
#define     MEMCLKCTL1_RAM7CLKSEL                                             13
#define     MEMCLKCTL1_RAM6CLKSEL                                             12

//--------------RTC-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     RTC_BASE                                                          0xC0120000
#define     RTC_CTL                                                           (RTC_BASE+0x00)
#define     RTC_REGUPDATA                                                     (RTC_BASE+0x04)
#define     RTC_DHMSALM                                                       (RTC_BASE+0x08)
#define     RTC_DHMS                                                          (RTC_BASE+0x0C)
#define     RTC_YMD                                                           (RTC_BASE+0x10)
#define     RTC_ACCESS                                                        (RTC_BASE+0x14)
#define     Hz2_CTL                                                           (RTC_BASE+0x18)
#define     WD_CTL                                                            (RTC_BASE+0x1c)
#define     T0_CTL                                                            (RTC_BASE+0x20)
#define     T0_VAL                                                            (RTC_BASE+0x24)
#define     T1_CTL                                                            (RTC_BASE+0x28)
#define     T1_VAL                                                            (RTC_BASE+0x2C)

//--------------Bits Location------------------------------------------//
#define     RTC_CTL_TEST_EN                                                   16
#define     RTC_CTL_LOSC_CAP_selection                                        13
#define     RTC_CTL_LOSC_RDY                                                  12
#define     RTC_CTL_LOSC_CAPSEL_e                                             11
#define     RTC_CTL_LOSC_CAPSEL_SHIFT                                         8
#define     RTC_CTL_LOSC_CAPSEL_MASK                                          (0xF<<8)
#define     RTC_CTL_LEAP                                                      7
#define     RTC_CTL_EOSC                                                      6
#define     RTC_CTL_CKSS0                                                     5
#define     RTC_CTL_CAL_EN                                                    4
#define     RTC_CTL_ALIE                                                      1
#define     RTC_CTL_ALIP                                                      0

#define     RTC_REGUPDATA_UPDATA_e                                            15
#define     RTC_REGUPDATA_UPDATA_SHIFT                                        0
#define     RTC_REGUPDATA_UPDATA_MASK                                         (0xFFFF<<0)

#define     RTC_DHMSALM_HOUEAL_e                                              20
#define     RTC_DHMSALM_HOUEAL_SHIFT                                          16
#define     RTC_DHMSALM_HOUEAL_MASK                                           (0x1F<<16)
#define     RTC_DHMSALM_MINAL_e                                               13
#define     RTC_DHMSALM_MINAL_SHIFT                                           8
#define     RTC_DHMSALM_MINAL_MASK                                            (0x3F<<8)
#define     RTC_DHMSALM_SECAL_e                                               5
#define     RTC_DHMSALM_SECAL_SHIFT                                           0
#define     RTC_DHMSALM_SECAL_MASK                                            (0x3F<<0)

#define     RTC_DHMS_HOUR_e                                                   20
#define     RTC_DHMS_HOUR_SHIFT                                               16
#define     RTC_DHMS_HOUR_MASK                                                (0x1F<<16)
#define     RTC_DHMS_MIN_e                                                    13
#define     RTC_DHMS_MIN_SHIFT                                                8
#define     RTC_DHMS_MIN_MASK                                                 (0x3F<<8)
#define     RTC_DHMS_SEC_e                                                    5
#define     RTC_DHMS_SEC_SHIFT                                                0
#define     RTC_DHMS_SEC_MASK                                                 (0x3F<<0)

#define     RTC_YMD_YEAR_e                                                    22
#define     RTC_YMD_YEAR_SHIFT                                                16
#define     RTC_YMD_YEAR_MASK                                                 (0x7F<<16)
#define     RTC_YMD_MON_e                                                     11
#define     RTC_YMD_MON_SHIFT                                                 8
#define     RTC_YMD_MON_MASK                                                  (0xF<<8)
#define     RTC_YMD_DATE_e                                                    4
#define     RTC_YMD_DATE_SHIFT                                                0
#define     RTC_YMD_DATE_MASK                                                 (0x1F<<0)

#define     RTC_ACCESS_ACCESS_e                                               7
#define     RTC_ACCESS_ACCESS_SHIFT                                           0
#define     RTC_ACCESS_ACCESS_MASK                                            (0xFF<<0)

#define     Hz2_CTL_CKSS1                                                     2
#define     Hz2_CTL_2HIE                                                      1
#define     Hz2_CTL_2HIP                                                      0

#define     WD_CTL_EJTAG_F                                                    31
#define     WD_CTL_IRQP                                                       6
#define     WD_CTL_SIGS                                                       5
#define     WD_CTL_WDEN                                                       4
#define     WD_CTL_CLKSEL_e                                                   3
#define     WD_CTL_CLKSEL_SHIFT                                               1
#define     WD_CTL_CLKSEL_MASK                                                (0x7<<1)
#define     WD_CTL_CLR                                                        0

#define     T0_CTL_EN                                                         5
#define     T0_CTL_RELO                                                       2
#define     T0_CTL_ZIEN                                                       1
#define     T0_CTL_ZIPD                                                       0

#define     T0_VAL_T0_e                                                       23
#define     T0_VAL_T0_SHIFT                                                   0
#define     T0_VAL_T0_MASK                                                    (0xFFFFFF<<0)

#define     T1_CTL_En                                                         5
#define     T1_CTL_RELO                                                       2
#define     T1_CTL_ZIEN                                                       1
#define     T1_CTL_ZIPD                                                       0

#define     T1_VAL_T1_e                                                       23
#define     T1_VAL_T1_SHIFT                                                   0
#define     T1_VAL_T1_MASK                                                    (0xFFFFFF<<0)

//--------------InterruptController-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     InterruptController_BASE                                          0xC00B0000
#define     INTC_PD                                                           (InterruptController_BASE+0x00000000)
#define     INTC_MSK                                                          (InterruptController_BASE+0x00000004)
#define     INTC_CFG0                                                         (InterruptController_BASE+0x00000008)
#define     INTC_CFG1                                                         (InterruptController_BASE+0x0000000c)
#define     INTC_CFG2                                                         (InterruptController_BASE+0x00000010)
#define     INTC_EXTCTL                                                       (InterruptController_BASE+0x00000014)
#define     INTC_EXTIP                                                        (InterruptController_BASE+0x00000018)

//--------------Bits Location------------------------------------------//
#define     INTC_PD_DMA3_IP                                                   21
#define     INTC_PD_DMA2_IP                                                   20
#define     INTC_PD_DMA1_IP                                                   19
#define     INTC_PD_DMA0_IP                                                   18
#define     INTC_PD_SD_IP                                                     17
#define     INTC_PD_NAND_IP                                                   16
#define     INTC_PD_ADC_IP                                                    15
#define     INTC_PD_DAC_IIS_IP                                                14
#define     INTC_PD_SIRQ1_IP                                                  13
#define     INTC_PD_UART_IP                                                   12
#define     INTC_PD_IIC_IP                                                    11
#define     INTC_PD_USB_IP                                                    10
#define     INTC_PD_SPI_IP                                                    9
#define     INTC_PD_TouchKey_IP                                               8
#define     INTC_PD_SIRQ0_IP                                                  7
#define     INTC_PD_LRADC_IP                                                  6
#define     INTC_PD_RTC_IP                                                    5
#define     INTC_PD_TIMER0_IP                                                 4
#define     INTC_PD_TIMER1_IP                                                 3
#define     INTC_PD_2Hz_IP                                                    2
#define     INTC_PD_AE_IP                                                     1
#define     INTC_PD_MJPEG_IP                                                  0

#define     INTC_MSK_DMA3_IM                                                  21
#define     INTC_MSK_DMA2_IM                                                  20
#define     INTC_MSK_DMA1_IM                                                  19
#define     INTC_MSK_DMA0_IM                                                  18
#define     INTC_MSK_SD_IM                                                    17
#define     INTC_MSK_NAND_IM                                                  16
#define     INTC_MSK_ADC_IM                                                   15
#define     INTC_MSK_DAC_IIS_IM                                               14
#define     INTC_MSK_SIRQ1_IM                                                 13
#define     INTC_MSK_UART_IM                                                  12
#define     INTC_MSK_IIC_IM                                                   11
#define     INTC_MSK_USB_IM                                                   10
#define     INTC_MSK_SPI_IM                                                   9
#define     INTC_MSK_TouchKey_IM                                              8
#define     INTC_MSK_SIRQ0_IM                                                 7
#define     INTC_MSK_LRADC_IM                                                 6
#define     INTC_MSK_RTC_IM                                                   5
#define     INTC_MSK_TIMER0_IM                                                4
#define     INTC_MSK_TIMER1_IM                                                3
#define     INTC_MSK_2Hz_IM                                                   2
#define     INTC_MSK_AE_IM                                                    1
#define     INTC_MSK_MJPEG_IM                                                 0

#define     INTC_CFG0_DMA3_CFG0                                               21
#define     INTC_CFG0_DMA2_CFG0                                               20
#define     INTC_CFG0_DMA1_CFG0                                               19
#define     INTC_CFG0_DMA0_CFG0                                               18
#define     INTC_CFG0_SD_CFG0                                                 17
#define     INTC_CFG0_NAND_CFG0                                               16
#define     INTC_CFG0_ADC_CFG0                                                15
#define     INTC_CFG0_DAC_CFG0                                                14
#define     INTC_CFG0_SIRQ1_CFG0                                              13
#define     INTC_CFG0_UART_CFG0                                               12
#define     INTC_CFG0_IIC_CFG0                                                11
#define     INTC_CFG0_USB_CFG0                                                10
#define     INTC_CFG0_SPI_CFG0                                                9
#define     INTC_CFG0_TouchKey_CFG0                                           8
#define     INTC_CFG0_SIRQ0_CFG0                                              7
#define     INTC_CFG0_LRADC_CFG0                                              6
#define     INTC_CFG0_RTC_CFG0                                                5
#define     INTC_CFG0_TIMER0_CFG0                                             4
#define     INTC_CFG0_TIMER1_CFG0                                             3
#define     INTC_CFG0_2Hz_CFG0                                                2
#define     INTC_CFG0_AE_CFG0                                                 1
#define     INTC_CFG0_MJPEG_CFG0                                              0

#define     INTC_CFG1_DMA3_CFG1                                               21
#define     INTC_CFG1_DMA2_CFG1                                               20
#define     INTC_CFG1_DMA1_CFG1                                               19
#define     INTC_CFG1_DMA0_CFG1                                               18
#define     INTC_CFG1_SD_CFG1                                                 17
#define     INTC_CFG1_NAND_CFG1                                               16
#define     INTC_CFG1_ADC_CFG1                                                15
#define     INTC_CFG1_DAC_CFG1                                                14
#define     INTC_CFG1_SIRQ1_CFG1                                              13
#define     INTC_CFG1_UART_CFG1                                               12
#define     INTC_CFG1_IIC_CFG1                                                11
#define     INTC_CFG1_USB_CFG1                                                10
#define     INTC_CFG1_SPI_CFG1                                                9
#define     INTC_CFG1_TouchKey_CFG1                                           8
#define     INTC_CFG1_SIRQ0_CFG1                                              7
#define     INTC_CFG1_LRADC_CFG1                                              6
#define     INTC_CFG1_RTC_CFG1                                                5
#define     INTC_CFG1_TIMER0_CFG1                                             4
#define     INTC_CFG1_TIMER1_CFG1                                             3
#define     INTC_CFG1_2Hz_CFG1                                                2
#define     INTC_CFG1_AE_CFG1                                                 1
#define     INTC_CFG1_MJPEG_CFG1                                              0

#define     INTC_CFG2_DMA3_CFG2                                               21
#define     INTC_CFG2_DMA2_CFG2                                               20
#define     INTC_CFG2_DMA1_CFG2                                               19
#define     INTC_CFG2_DMA0_CFG2                                               18
#define     INTC_CFG2_SD_CFG2                                                 17
#define     INTC_CFG2_NAND_CFG2                                               16
#define     INTC_CFG2_ADC_CFG2                                                15
#define     INTC_CFG2_DAC_CFG2                                                14
#define     INTC_CFG2_SIRQ1_CFG2                                              13
#define     INTC_CFG2_UART_CFG2                                               12
#define     INTC_CFG2_IIC_CFG2                                                11
#define     INTC_CFG2_USB_CFG2                                                10
#define     INTC_CFG2_SPI_CFG2                                                9
#define     INTC_CFG2_TouchKey_CFG2                                           8
#define     INTC_CFG2_SIRQ0_CFG2                                              7
#define     INTC_CFG2_LRADC_CFG2                                              6
#define     INTC_CFG2_RTC_CFG2                                                5
#define     INTC_CFG2_TIMER0_CFG2                                             4
#define     INTC_CFG2_TIMER1_CFG2                                             3
#define     INTC_CFG2_2Hz_CFG2                                                2
#define     INTC_CFG2_AE_CFG2                                                 1
#define     INTC_CFG2_MJPEG_CFG2                                              0

#define     INTC_EXTCTL_EXTYPE1                                               3
#define     INTC_EXTCTL_EXTYPE0                                               1

#define     INTC_EXTIP_E1PD                                                   1
#define     INTC_EXTIP_E0PD                                                   0

//--------------DMAController-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     DMAController_BASE                                                0xC00C0000
#define     DMAPriority                                                       (DMAController_BASE+0x00000000)
#define     DMAIP                                                             (DMAController_BASE+0x00000004)
#define     DMAIE                                                             (DMAController_BASE+0x00000008)
#define     DMADBG                                                            (DMAController_BASE+0x0000000c)
#define     DMA0CTL                                                           (DMAController_BASE+0x00000010)
#define     DMA0SADDR0                                                        (DMAController_BASE+0x00000014)
#define     DMA0SADDR1                                                        (DMAController_BASE+0x00000018)
#define     DMA0SADDR2                                                        (DMAController_BASE+0x0000001c)
#define     DMA0DADDR0                                                        (DMAController_BASE+0x00000020)
#define     DMA0DADDR1                                                        (DMAController_BASE+0x00000024)
#define     DMA0FrameLen                                                      (DMAController_BASE+0x00000028)
#define     DMA0LCDCFG                                                        (DMAController_BASE+0x0000002c)
#define     DMA1CTL                                                           (DMAController_BASE+0x00000030)
#define     DMA1SADDR0                                                        (DMAController_BASE+0x00000034)
#define     DMA1SADDR1                                                        (DMAController_BASE+0x00000038)
#define     DMA1SADDR2                                                        (DMAController_BASE+0x0000003c)
#define     DMA1DADDR0                                                        (DMAController_BASE+0x00000040)
#define     DMA1DADDR1                                                        (DMAController_BASE+0x00000044)
#define     DMA1FrameLen                                                      (DMAController_BASE+0x00000048)
#define     DMA1LCDCFG                                                        (DMAController_BASE+0x0000004c)
#define     DMA2CTL                                                           (DMAController_BASE+0x00000050)
#define     DMA2SADDR0                                                        (DMAController_BASE+0x00000054)
#define     DMA2SADDR1                                                        (DMAController_BASE+0x00000058)
#define     DMA2SADDR2                                                        (DMAController_BASE+0x0000005c)
#define     DMA2DADDR0                                                        (DMAController_BASE+0x00000060)
#define     DMA2DADDR1                                                        (DMAController_BASE+0x00000064)
#define     DMA2FrameLen                                                      (DMAController_BASE+0x00000068)
#define     DMA2LCDCFG                                                        (DMAController_BASE+0x0000006c)
#define     DMA3CTL                                                           (DMAController_BASE+0x00000070)
#define     DMA3SADDR0                                                        (DMAController_BASE+0x00000074)
#define     DMA3SADDR1                                                        (DMAController_BASE+0x00000078)
#define     DMA3SADDR2                                                        (DMAController_BASE+0x0000007c)
#define     DMA3DADDR0                                                        (DMAController_BASE+0x00000080)
#define     DMA3DADDR1                                                        (DMAController_BASE+0x00000084)
#define     DMA3FrameLen                                                      (DMAController_BASE+0x00000088)
#define     DMA3LCDCFG                                                        (DMAController_BASE+0x0000008c)

//--------------Bits Location------------------------------------------//
#define     DMAPriority_PriorityTAB_e                                         4
#define     DMAPriority_PriorityTAB_SHIFT                                     0
#define     DMAPriority_PriorityTAB_MASK                                      (0x1F<<0)

#define     DMAIP_DMA3HFIP                                                    7
#define     DMAIP_DMA2HFIP                                                    6
#define     DMAIP_DMA1HFIP                                                    5
#define     DMAIP_DMA0HFIP                                                    4
#define     DMAIP_DMA3TCIP                                                    3
#define     DMAIP_DMA2TCIP                                                    2
#define     DMAIP_DMA1TCIP                                                    1
#define     DMAIP_DMA0TCIP                                                    0

#define     DMAIE_DMA3HFIE                                                    7
#define     DMAIE_DMA2HFIE                                                    6
#define     DMAIE_DMA1HFIE                                                    5
#define     DMAIE_DMA0HFIE                                                    4
#define     DMAIE_DMA3TCIE                                                    3
#define     DMAIE_DMA2TCIE                                                    2
#define     DMAIE_DMA1TCIE                                                    1
#define     DMAIE_DMA0TCIE                                                    0

#define     DMADBG_DMADBG_e                                                   2
#define     DMADBG_DMADBG_SHIFT                                               0
#define     DMADBG_DMADBG_MASK                                                (0x7<<0)

#define     DMA0CTL_AUDIOTYPE                                                 16
#define     DMA0CTL_DATAWIDTH_e                                               15
#define     DMA0CTL_DATAWIDTH_SHIFT                                           14
#define     DMA0CTL_DATAWIDTH_MASK                                            (0x3<<14)
#define     DMA0CTL_LCDMODE_e                                                 13
#define     DMA0CTL_LCDMODE_SHIFT                                             12
#define     DMA0CTL_LCDMODE_MASK                                              (0x3<<12)
#define     DMA0CTL_DSTTYPE_e                                                 11
#define     DMA0CTL_DSTTYPE_SHIFT                                             8
#define     DMA0CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA0CTL_SRCTYPE_e                                                 7
#define     DMA0CTL_SRCTYPE_SHIFT                                             4
#define     DMA0CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA0CTL_reload                                                    1
#define     DMA0CTL_DMA0START                                                 0

#define     DMA0SADDR0_DMA0SADDR0_e                                           17
#define     DMA0SADDR0_DMA0SADDR0_SHIFT                                       0
#define     DMA0SADDR0_DMA0SADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA0SADDR1_DMA0SADDR1_e                                           17
#define     DMA0SADDR1_DMA0SADDR1_SHIFT                                       0
#define     DMA0SADDR1_DMA0SADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA0SADDR2_DMA0SADDR2_e                                           17
#define     DMA0SADDR2_DMA0SADDR2_SHIFT                                       0
#define     DMA0SADDR2_DMA0SADDR2_MASK                                        (0x3FFFF<<0)

#define     DMA0DADDR0_DMA0DADDR0_e                                           17
#define     DMA0DADDR0_DMA0DADDR0_SHIFT                                       0
#define     DMA0DADDR0_DMA0DADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA0DADDR1_DMA0DADDR1_e                                           17
#define     DMA0DADDR1_DMA0DADDR1_SHIFT                                       0
#define     DMA0DADDR1_DMA0DADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA0FrameLen_DMA0FrameLen_e                                       17
#define     DMA0FrameLen_DMA0FrameLen_SHIFT                                   0
#define     DMA0FrameLen_DMA0FrameLen_MASK                                    (0x3FFFF<<0)

#define     DMA0LCDCFG_LCDWIDTH_e                                             8
#define     DMA0LCDCFG_LCDWIDTH_SHIFT                                         0
#define     DMA0LCDCFG_LCDWIDTH_MASK                                          (0x1FF<<0)

#define     DMA1CTL_AUDIOTYPE                                                 16
#define     DMA1CTL_DATAWIDTH_e                                               15
#define     DMA1CTL_DATAWIDTH_SHIFT                                           14
#define     DMA1CTL_DATAWIDTH_MASK                                            (0x3<<14)
#define     DMA1CTL_LCDMODE_e                                                 13
#define     DMA1CTL_LCDMODE_SHIFT                                             12
#define     DMA1CTL_LCDMODE_MASK                                              (0x3<<12)
#define     DMA1CTL_DSTTYPE_e                                                 11
#define     DMA1CTL_DSTTYPE_SHIFT                                             8
#define     DMA1CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA1CTL_SRCTYPE_e                                                 7
#define     DMA1CTL_SRCTYPE_SHIFT                                             4
#define     DMA1CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA1CTL_reload                                                    1
#define     DMA1CTL_DMA1START                                                 0

#define     DMA1SADDR0_DMA1SADDR0_e                                           17
#define     DMA1SADDR0_DMA1SADDR0_SHIFT                                       0
#define     DMA1SADDR0_DMA1SADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA1SADDR1_DMA1SADDR1_e                                           17
#define     DMA1SADDR1_DMA1SADDR1_SHIFT                                       0
#define     DMA1SADDR1_DMA1SADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA1SADDR2_DMA1SADDR2_e                                           17
#define     DMA1SADDR2_DMA1SADDR2_SHIFT                                       0
#define     DMA1SADDR2_DMA1SADDR2_MASK                                        (0x3FFFF<<0)

#define     DMA1DADDR0_DMA1DADDR0_e                                           17
#define     DMA1DADDR0_DMA1DADDR0_SHIFT                                       0
#define     DMA1DADDR0_DMA1DADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA1DADDR1_DMA1DADDR1_e                                           17
#define     DMA1DADDR1_DMA1DADDR1_SHIFT                                       0
#define     DMA1DADDR1_DMA1DADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA1FrameLen_DMA1FrameLen_e                                       17
#define     DMA1FrameLen_DMA1FrameLen_SHIFT                                   0
#define     DMA1FrameLen_DMA1FrameLen_MASK                                    (0x3FFFF<<0)

#define     DMA1LCDCFG_LCDWIDTH_e                                             8
#define     DMA1LCDCFG_LCDWIDTH_SHIFT                                         0
#define     DMA1LCDCFG_LCDWIDTH_MASK                                          (0x1FF<<0)

#define     DMA2CTL_AUDIOTYPE                                                 16
#define     DMA2CTL_DATAWIDTH_e                                               15
#define     DMA2CTL_DATAWIDTH_SHIFT                                           14
#define     DMA2CTL_DATAWIDTH_MASK                                            (0x3<<14)
#define     DMA2CTL_LCDMODE_e                                                 13
#define     DMA2CTL_LCDMODE_SHIFT                                             12
#define     DMA2CTL_LCDMODE_MASK                                              (0x3<<12)
#define     DMA2CTL_DSTTYPE_e                                                 11
#define     DMA2CTL_DSTTYPE_SHIFT                                             8
#define     DMA2CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA2CTL_SRCTYPE_e                                                 7
#define     DMA2CTL_SRCTYPE_SHIFT                                             4
#define     DMA2CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA2CTL_reload                                                    1
#define     DMA2CTL_DMA2START                                                 0

#define     DMA2SADDR0_DMA2SADDR0_e                                           17
#define     DMA2SADDR0_DMA2SADDR0_SHIFT                                       0
#define     DMA2SADDR0_DMA2SADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA2SADDR1_DMA2SADDR1_e                                           17
#define     DMA2SADDR1_DMA2SADDR1_SHIFT                                       0
#define     DMA2SADDR1_DMA2SADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA2SADDR2_DMA2SADDR2_e                                           17
#define     DMA2SADDR2_DMA2SADDR2_SHIFT                                       0
#define     DMA2SADDR2_DMA2SADDR2_MASK                                        (0x3FFFF<<0)

#define     DMA2DADDR0_DMA2DADDR0_e                                           17
#define     DMA2DADDR0_DMA2DADDR0_SHIFT                                       0
#define     DMA2DADDR0_DMA2DADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA2DADDR1_DMA2DADDR1_e                                           17
#define     DMA2DADDR1_DMA2DADDR1_SHIFT                                       0
#define     DMA2DADDR1_DMA2DADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA2FrameLen_DMA2FrameLen_e                                       17
#define     DMA2FrameLen_DMA2FrameLen_SHIFT                                   0
#define     DMA2FrameLen_DMA2FrameLen_MASK                                    (0x3FFFF<<0)

#define     DMA2LCDCFG_LCDWIDTH_e                                             8
#define     DMA2LCDCFG_LCDWIDTH_SHIFT                                         0
#define     DMA2LCDCFG_LCDWIDTH_MASK                                          (0x1FF<<0)

#define     DMA3CTL_AUDIOTYPE                                                 16
#define     DMA3CTL_DATAWIDTH_e                                               15
#define     DMA3CTL_DATAWIDTH_SHIFT                                           14
#define     DMA3CTL_DATAWIDTH_MASK                                            (0x3<<14)
#define     DMA3CTL_LCDMODE_e                                                 13
#define     DMA3CTL_LCDMODE_SHIFT                                             12
#define     DMA3CTL_LCDMODE_MASK                                              (0x3<<12)
#define     DMA3CTL_DSTTYPE_e                                                 11
#define     DMA3CTL_DSTTYPE_SHIFT                                             8
#define     DMA3CTL_DSTTYPE_MASK                                              (0xF<<8)
#define     DMA3CTL_SRCTYPE_e                                                 7
#define     DMA3CTL_SRCTYPE_SHIFT                                             4
#define     DMA3CTL_SRCTYPE_MASK                                              (0xF<<4)
#define     DMA3CTL_reload                                                    1
#define     DMA3CTL_DMA3START                                                 0

#define     DMA3SADDR0_DMA3SADDR0_e                                           17
#define     DMA3SADDR0_DMA3SADDR0_SHIFT                                       0
#define     DMA3SADDR0_DMA3SADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA3SADDR1_DMA3SADDR1_e                                           17
#define     DMA3SADDR1_DMA3SADDR1_SHIFT                                       0
#define     DMA3SADDR1_DMA3SADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA3SADDR2_DMA3SADDR2_e                                           17
#define     DMA3SADDR2_DMA3SADDR2_SHIFT                                       0
#define     DMA3SADDR2_DMA3SADDR2_MASK                                        (0x3FFFF<<0)

#define     DMA3DADDR0_DMA3DADDR0_e                                           17
#define     DMA3DADDR0_DMA3DADDR0_SHIFT                                       0
#define     DMA3DADDR0_DMA3DADDR0_MASK                                        (0x3FFFF<<0)

#define     DMA3DADDR1_DMA3DADDR1_e                                           17
#define     DMA3DADDR1_DMA3DADDR1_SHIFT                                       0
#define     DMA3DADDR1_DMA3DADDR1_MASK                                        (0x3FFFF<<0)

#define     DMA3FrameLen_DMA3FrameLen_e                                       17
#define     DMA3FrameLen_DMA3FrameLen_SHIFT                                   0
#define     DMA3FrameLen_DMA3FrameLen_MASK                                    (0x3FFFF<<0)

#define     DMA3LCDCFG_LCDWIDTH_e                                             8
#define     DMA3LCDCFG_LCDWIDTH_SHIFT                                         0
#define     DMA3LCDCFG_LCDWIDTH_MASK                                          (0x1FF<<0)

//--------------AUIP-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     AUIP_BASE                                                         0xC0150000
#define     AuCodecCtl                                                        (AUIP_BASE+0x00000000)
#define     AuDebugLength                                                     (AUIP_BASE+0x00000004)
#define     AuCodecDebug                                                      (AUIP_BASE+0x00000008)
#define     AuCodecFIFOCtl                                                    (AUIP_BASE+0x0000000c)
#define     AuCodecFIFOData                                                   (AUIP_BASE+0x00000010)
#define     AuCodecDecState                                                   (AUIP_BASE+0x00000014)
#define     MP3IE                                                             (AUIP_BASE+0x00000018)
#define     MP3IP                                                             (AUIP_BASE+0x0000001c)
#define     MP3Ctl                                                            (AUIP_BASE+0x00000020)
#define     MP3HeaderInfo                                                     (AUIP_BASE+0x00000024)
#define     MP3BitRate                                                        (AUIP_BASE+0x00000028)
#define     MP3TTimeH                                                         (AUIP_BASE+0x0000002c)
#define     MP3TTimeM                                                         (AUIP_BASE+0x00000030)
#define     MP3TTimeS                                                         (AUIP_BASE+0x00000034)
#define     MP3CTimeH                                                         (AUIP_BASE+0x00000038)
#define     MP3CTimeM                                                         (AUIP_BASE+0x0000003c)
#define     MP3CTimeS                                                         (AUIP_BASE+0x00000040)
#define     MP3FrameNum                                                       (AUIP_BASE+0x00000044)
#define     MP3FileLen                                                        (AUIP_BASE+0x00000048)
#define     MP3HeaderSynCtl                                                   (AUIP_BASE+0x0000004c)
#define     SynHeaderData                                                     (AUIP_BASE+0x00000050)
#define     WMAIE                                                             (AUIP_BASE+0x00000018)
#define     WMAIP                                                             (AUIP_BASE+0x0000001c)
#define     WMACtl                                                            (AUIP_BASE+0x00000020)
#define     WMAHeaderInfo                                                     (AUIP_BASE+0x00000024)
#define     WMABitRate                                                        (AUIP_BASE+0x00000028)
#define     WMATTimeH                                                         (AUIP_BASE+0x0000002c)
#define     WMATTimeM                                                         (AUIP_BASE+0x00000030)
#define     WMATTimeS                                                         (AUIP_BASE+0x00000034)
#define     WMACTimeH                                                         (AUIP_BASE+0x00000038)
#define     WMACTimeM                                                         (AUIP_BASE+0x0000003c)
#define     WMACTimeS                                                         (AUIP_BASE+0x00000040)
#define     WMAPackNum                                                        (AUIP_BASE+0x00000044)
#define     WMAPackSize                                                       (AUIP_BASE+0x00000048)
#define     WAVIE                                                             (AUIP_BASE+0x00000018)
#define     WAVIP                                                             (AUIP_BASE+0x0000001c)
#define     WAVCtl                                                            (AUIP_BASE+0x00000020)
#define     WAVHeaderInfo                                                     (AUIP_BASE+0x00000024)
#define     WAVBitRate                                                        (AUIP_BASE+0x00000028)
#define     WAVTTimeH                                                         (AUIP_BASE+0x0000002c)
#define     WAVTTimeM                                                         (AUIP_BASE+0x00000030)
#define     WAVTTimeS                                                         (AUIP_BASE+0x00000034)
#define     WAVCTimeH                                                         (AUIP_BASE+0x00000038)
#define     WAVCTimeM                                                         (AUIP_BASE+0x0000003c)
#define     WAVCTimeS                                                         (AUIP_BASE+0x00000040)
#define     WAVBlockNum                                                       (AUIP_BASE+0x00000044)
#define     FLACIE                                                            (AUIP_BASE+0x00000018)
#define     FLACIP                                                            (AUIP_BASE+0x0000001c)
#define     FLACCtl                                                           (AUIP_BASE+0x00000020)
#define     FLACHeaderInfo                                                    (AUIP_BASE+0x00000024)
#define     METADATAInfo                                                      (AUIP_BASE+0x00000028)
#define     FLACTTimeH                                                        (AUIP_BASE+0x0000002c)
#define     FLACTTimeM                                                        (AUIP_BASE+0x00000030)
#define     FLACTTimeS                                                        (AUIP_BASE+0x00000034)
#define     FLACCTimeH                                                        (AUIP_BASE+0x00000038)
#define     FLACCTimeM                                                        (AUIP_BASE+0x0000003c)
#define     FLACCTimeS                                                        (AUIP_BASE+0x00000040)
#define     FLACErrInfo                                                       (AUIP_BASE+0x00000044)
#define     CurrentPos                                                        (AUIP_BASE+0x00000048)
#define     FLACCFrameNum                                                     (AUIP_BASE+0x0000004c)
#define     FLACHeaderSynCtl                                                  (AUIP_BASE+0x00000050)
#define     APEIE                                                             (AUIP_BASE+0x00000018)
#define     APEIP                                                             (AUIP_BASE+0x0000001c)
#define     APECtl                                                            (AUIP_BASE+0x00000020)
#define     APEHeaderInfo1                                                    (AUIP_BASE+0x00000024)
#define     APEHeaderInfo2                                                    (AUIP_BASE+0x00000028)
#define     APETTimeH                                                         (AUIP_BASE+0x0000002c)
#define     APETTimeM                                                         (AUIP_BASE+0x00000030)
#define     APETTimeS                                                         (AUIP_BASE+0x00000034)
#define     APECTimeH                                                         (AUIP_BASE+0x00000038)
#define     APECTimeM                                                         (AUIP_BASE+0x0000003c)
#define     APECTimeS                                                         (AUIP_BASE+0x00000040)
#define     APEErrInfo                                                        (AUIP_BASE+0x00000044)
#define     APECFrameNum                                                      (AUIP_BASE+0x00000048)
#define     SoftIE                                                            (AUIP_BASE+0x00000018)
#define     SoftIP                                                            (AUIP_BASE+0x0000001c)
#define     SoftCtl                                                           (AUIP_BASE+0x00000020)
#define     FrameLen                                                          (AUIP_BASE+0x00000024)
#define     BEPCtl1                                                           (AUIP_BASE+0x00000054)
#define     BEPCtl2                                                           (AUIP_BASE+0x00000058)
#define     BEPCtl3                                                           (AUIP_BASE+0x0000005c)
#define     BEPCtl4                                                           (AUIP_BASE+0x00000060)
#define     BEPCtl5                                                           (AUIP_BASE+0x00000064)
#define     GlobalGain                                                        (AUIP_BASE+0x00000068)
#define     CurrentEnergy                                                     (AUIP_BASE+0x0000006c)
#define     KaraokeCtl                                                        (AUIP_BASE+0x00000074)
#define     LCHGain                                                           (AUIP_BASE+0x00000078)
#define     RCHGain                                                           (AUIP_BASE+0x0000007c)
#define     LPFGain                                                           (AUIP_BASE+0x00000080)
#define     HPFGain                                                           (AUIP_BASE+0x00000084)
#define     EQCoe0                                                            (AUIP_BASE+0x00000088)
#define     EQCoe1                                                            (AUIP_BASE+0x0000008c)
#define     EQCoe2                                                            (AUIP_BASE+0x00000090)
#define     EQCoe3                                                            (AUIP_BASE+0x00000094)
#define     EQCoe4                                                            (AUIP_BASE+0x00000098)
#define     EQCoe5                                                            (AUIP_BASE+0x0000009c)
#define     EQCoe6                                                            (AUIP_BASE+0x000000a0)
#define     SRSCtl                                                            (AUIP_BASE+0x000000a4)
#define     SRSCfg1                                                           (AUIP_BASE+0x000000a8)
#define     SRSCfg2                                                           (AUIP_BASE+0x000000ac)
#define     SRSCfg3                                                           (AUIP_BASE+0x000000b0)
#define     SRSCfg4                                                           (AUIP_BASE+0x000000b4)
#define     VPSCtl                                                            (AUIP_BASE+0x000000b8)
#define     SequenceSize                                                      (AUIP_BASE+0x000000bc)
#define     OverlapSize                                                       (AUIP_BASE+0x000000c0)
#define     SeekWindowSize                                                    (AUIP_BASE+0x000000c4)

//--------------Bits Location------------------------------------------//
#define     AuCodecCtl_Reset                                                  7
#define     AuCodecCtl_Mode_e                                                 6
#define     AuCodecCtl_Mode_SHIFT                                             4
#define     AuCodecCtl_Mode_MASK                                              (0x7<<4)
#define     AuCodecCtl_EnableAllClk                                           3
#define     AuCodecCtl_AHB_DMA_SEL                                            0

#define     AuDebugLength_PcmLen_e                                            12
#define     AuDebugLength_PcmLen_SHIFT                                        0
#define     AuDebugLength_PcmLen_MASK                                         (0x1FFF<<0)

#define     AuCodecDebug_DebugMode                                            7
#define     AuCodecDebug_DebugState                                           6
#define     AuCodecDebug_Halt2IE                                              5
#define     AuCodecDebug_Halt2IP                                              4
#define     AuCodecDebug_PowerSave                                            3
#define     AuCodecDebug_DEBUG_EN                                             2
#define     AuCodecDebug_DebugSel                                             1
#define     AuCodecDebug_DecEn                                                0

#define     AuCodecFIFOCtl_Empty                                              7
#define     AuCodecFIFOCtl_AlmostEmpty                                        6
#define     AuCodecFIFOCtl_OverFlow                                           5
#define     AuCodecFIFOCtl_DRQLevel_e                                         4
#define     AuCodecFIFOCtl_DRQLevel_SHIFT                                     1
#define     AuCodecFIFOCtl_DRQLevel_MASK                                      (0xF<<1)
#define     AuCodecFIFOCtl_ResetFIFO                                          0

#define     AuCodecFIFOData_FIFOData_e                                        7
#define     AuCodecFIFOData_FIFOData_SHIFT                                    0
#define     AuCodecFIFOData_FIFOData_MASK                                     (0xFF<<0)

#define     AuCodecDecState_State_e                                           15
#define     AuCodecDecState_State_SHIFT                                       0
#define     AuCodecDecState_State_MASK                                        (0xFFFF<<0)

#define     MP3IE_LackPcmEn                                                   7
#define     MP3IE_EnergyEn                                                    6
#define     MP3IE_WordIntEn                                                   5
#define     MP3IE_InfoChangeEn2                                               4
#define     MP3IE_InfoChangeEn1                                               3
#define     MP3IE_CRCErrIntEn                                                 2
#define     MP3IE_InfoRdyIntEn                                                1
#define     MP3IE_PCMRdyIntEn                                                 0

#define     MP3IP_LackIP                                                      7
#define     MP3IP_EnergyIP                                                    6
#define     MP3IP_WordIP                                                      5
#define     MP3IP_InfoChangeIP2                                               4
#define     MP3IP_InfoChangeIP1                                               3
#define     MP3IP_CRCErrIP                                                    2
#define     MP3IP_InfoRdyIP                                                   1
#define     MP3IP_PCMRdyIP                                                    0

#define     MP3Ctl_NewFile                                                    7
#define     MP3Ctl_En                                                         6
#define     MP3Ctl_FFEn                                                       5
#define     MP3Ctl_REVEn                                                      4
#define     MP3Ctl_CRCSkipEn                                                  3
#define     MP3Ctl_BKPlayEn                                                   1

#define     MP3HeaderInfo_SampFrq_e                                           7
#define     MP3HeaderInfo_SampFrq_SHIFT                                       4
#define     MP3HeaderInfo_SampFrq_MASK                                        (0xF<<4)
#define     MP3HeaderInfo_Layer_e                                             3
#define     MP3HeaderInfo_Layer_SHIFT                                         2
#define     MP3HeaderInfo_Layer_MASK                                          (0x3<<2)
#define     MP3HeaderInfo_Errprotect                                          1
#define     MP3HeaderInfo_AudioMode                                           0

#define     MP3BitRate_BitRateIdx3_e                                          15
#define     MP3BitRate_BitRateIdx3_SHIFT                                      12
#define     MP3BitRate_BitRateIdx3_MASK                                       (0xF<<12)
#define     MP3BitRate_BitRateIdx2_e                                          11
#define     MP3BitRate_BitRateIdx2_SHIFT                                      8
#define     MP3BitRate_BitRateIdx2_MASK                                       (0xF<<8)
#define     MP3BitRate_BitRateIdx1_e                                          7
#define     MP3BitRate_BitRateIdx1_SHIFT                                      4
#define     MP3BitRate_BitRateIdx1_MASK                                       (0xF<<4)
#define     MP3BitRate_BitRateIdx0_e                                          3
#define     MP3BitRate_BitRateIdx0_SHIFT                                      0
#define     MP3BitRate_BitRateIdx0_MASK                                       (0xF<<0)

#define     MP3TTimeH_XingTag                                                 7
#define     MP3TTimeH_TTimeH_e                                                5
#define     MP3TTimeH_TTimeH_SHIFT                                            0
#define     MP3TTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     MP3TTimeM_TTimeM_e                                                5
#define     MP3TTimeM_TTimeM_SHIFT                                            0
#define     MP3TTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     MP3TTimeS_TTimeS_e                                                5
#define     MP3TTimeS_TTimeS_SHIFT                                            0
#define     MP3TTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     MP3CTimeH_CTimeH_e                                                5
#define     MP3CTimeH_CTimeH_SHIFT                                            0
#define     MP3CTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     MP3CTimeM_CTimeM_e                                                5
#define     MP3CTimeM_CTimeM_SHIFT                                            0
#define     MP3CTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     MP3CTimeS_CTimeS_e                                                5
#define     MP3CTimeS_CTimeS_SHIFT                                            0
#define     MP3CTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     MP3FrameNum_FrameNum_e                                            23
#define     MP3FrameNum_FrameNum_SHIFT                                        0
#define     MP3FrameNum_FrameNum_MASK                                         (0xFFFFFF<<0)

#define     MP3FileLen_FileLength_e                                           31
#define     MP3FileLen_FileLength_SHIFT                                       8
#define     MP3FileLen_FileLength_MASK                                        (0xFFFFFF<<8)

#define     MP3HeaderSynCtl_IDEn                                              7
#define     MP3HeaderSynCtl_LayerEn                                           6
#define     MP3HeaderSynCtl_ProtectionBitEn                                   5
#define     MP3HeaderSynCtl_SampFreqEn                                        4
#define     MP3HeaderSynCtl_ModeEn                                            3
#define     MP3HeaderSynCtl_CopyrightEn                                       2
#define     MP3HeaderSynCtl_OriginalEn                                        1
#define     MP3HeaderSynCtl_EmphasisEn                                        0

#define     SynHeaderData_copyright                                           15
#define     SynHeaderData_original_copy                                       14
#define     SynHeaderData_emphasis_e                                          13
#define     SynHeaderData_emphasis_SHIFT                                      12
#define     SynHeaderData_emphasis_MASK                                       (0x3<<12)
#define     SynHeaderData_Copyright_r                                         11
#define     SynHeaderData_Original_copy_r                                     10
#define     SynHeaderData_Emphasis_r_e                                        9
#define     SynHeaderData_Emphasis_r_SHIFT                                    8
#define     SynHeaderData_Emphasis_r_MASK                                     (0x3<<8)
#define     SynHeaderData_SampFrq_e                                           7
#define     SynHeaderData_SampFrq_SHIFT                                       4
#define     SynHeaderData_SampFrq_MASK                                        (0xF<<4)
#define     SynHeaderData_Layer_e                                             3
#define     SynHeaderData_Layer_SHIFT                                         2
#define     SynHeaderData_Layer_MASK                                          (0x3<<2)
#define     SynHeaderData_Errprotect                                          1
#define     SynHeaderData_AudioMode                                           0

#define     WMAIE_LackIntEn                                                   7
#define     WMAIE_EnergyIntEn                                                 6
#define     WMAIE_WordIntEn                                                   5
#define     WMAIE_DRMInfoIntEn                                                3
#define     WMAIE_ErrIntEn                                                    2
#define     WMAIE_InfoRdyIntEn                                                1
#define     WMAIE_PCMRdyIntEn                                                 0

#define     WMAIP_LackIP                                                      7
#define     WMAIP_EnergyIP                                                    6
#define     WMAIP_WordIP                                                      5
#define     WMAIP_DRMInfoIP                                                   3
#define     WMAIP_ErrIP                                                       2
#define     WMAIP_InfoRdyIP                                                   1
#define     WMAIP_PCMRdyIP                                                    0

#define     WMACtl_NewFile                                                    7
#define     WMACtl_En                                                         6
#define     WMACtl_FF_REV_En                                                  5
#define     WMACtl_REVEn                                                      4
#define     WMACtl_REV_SEL                                                    1

#define     WMAHeaderInfo_SampFrq_e                                           7
#define     WMAHeaderInfo_SampFrq_SHIFT                                       4
#define     WMAHeaderInfo_SampFrq_MASK                                        (0xF<<4)
#define     WMAHeaderInfo_AllowSuper                                          3
#define     WMAHeaderInfo_WeightMode                                          2
#define     WMAHeaderInfo_NoiseSub                                            1
#define     WMAHeaderInfo_AudioMode                                           0

#define     WMABitRate_BitRateIdx3_e                                          15
#define     WMABitRate_BitRateIdx3_SHIFT                                      12
#define     WMABitRate_BitRateIdx3_MASK                                       (0xF<<12)
#define     WMABitRate_BitRateIdx2_e                                          11
#define     WMABitRate_BitRateIdx2_SHIFT                                      8
#define     WMABitRate_BitRateIdx2_MASK                                       (0xF<<8)
#define     WMABitRate_BitRateIdx1_e                                          7
#define     WMABitRate_BitRateIdx1_SHIFT                                      4
#define     WMABitRate_BitRateIdx1_MASK                                       (0xF<<4)
#define     WMABitRate_BitRateIdx0_e                                          3
#define     WMABitRate_BitRateIdx0_SHIFT                                      0
#define     WMABitRate_BitRateIdx0_MASK                                       (0xF<<0)

#define     WMATTimeH_TTimeH_e                                                5
#define     WMATTimeH_TTimeH_SHIFT                                            0
#define     WMATTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     WMATTimeM_TTimeM_e                                                5
#define     WMATTimeM_TTimeM_SHIFT                                            0
#define     WMATTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     WMATTimeS_TTimeS_e                                                5
#define     WMATTimeS_TTimeS_SHIFT                                            0
#define     WMATTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     WMACTimeH_CTimeH_e                                                5
#define     WMACTimeH_CTimeH_SHIFT                                            0
#define     WMACTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     WMACTimeM_CTimeM_e                                                5
#define     WMACTimeM_CTimeM_SHIFT                                            0
#define     WMACTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     WMACTimeS_CTimeS_e                                                5
#define     WMACTimeS_CTimeS_SHIFT                                            0
#define     WMACTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     WMAPackNum_PacketNum_e                                            23
#define     WMAPackNum_PacketNum_SHIFT                                        0
#define     WMAPackNum_PacketNum_MASK                                         (0xFFFFFF<<0)

#define     WMAPackSize_subfrszpre_e                                          23
#define     WMAPackSize_subfrszpre_SHIFT                                      19
#define     WMAPackSize_subfrszpre_MASK                                       (0x1F<<19)
#define     WMAPackSize_subfrszcurr_e                                         18
#define     WMAPackSize_subfrszcurr_SHIFT                                     14
#define     WMAPackSize_subfrszcurr_MASK                                      (0x1F<<14)
#define     WMAPackSize_discardlen_e                                          13
#define     WMAPackSize_discardlen_SHIFT                                      7
#define     WMAPackSize_discardlen_MASK                                       (0x7F<<7)
#define     WMAPackSize_PacketSize_e                                          6
#define     WMAPackSize_PacketSize_SHIFT                                      0
#define     WMAPackSize_PacketSize_MASK                                       (0x7F<<0)

#define     WAVIE_LackIntEn                                                   7
#define     WAVIE_EnergyIntEn                                                 6
#define     WAVIE_WordIntEn                                                   5
#define     WAVIE_ErrIntEn                                                    2
#define     WAVIE_InfoRdyIntEn                                                1
#define     WAVIE_PCMRdyIntEn                                                 0

#define     WAVIP_LackIP                                                      7
#define     WAVIP_EnergyIP                                                    6
#define     WAVIP_WordIP                                                      5
#define     WAVIP_ErrIP                                                       2
#define     WAVIP_InfoRdyIP                                                   1
#define     WAVIP_PCMRdyIP                                                    0

#define     WAVCtl_NewFile                                                    7
#define     WAVCtl_En                                                         6
#define     WAVCtl_FF_REV_En                                                  5
#define     WAVCtl_REVEn                                                      4
#define     WAVCtl_BKPlayEn                                                   1

#define     WAVHeaderInfo_SampFrq_e                                           7
#define     WAVHeaderInfo_SampFrq_SHIFT                                       4
#define     WAVHeaderInfo_SampFrq_MASK                                        (0xF<<4)
#define     WAVHeaderInfo_Format                                              1
#define     WAVHeaderInfo_AudioMode                                           0

#define     WAVBitRate_BitRateIdx3_e                                          15
#define     WAVBitRate_BitRateIdx3_SHIFT                                      12
#define     WAVBitRate_BitRateIdx3_MASK                                       (0xF<<12)
#define     WAVBitRate_BitRateIdx2_e                                          11
#define     WAVBitRate_BitRateIdx2_SHIFT                                      8
#define     WAVBitRate_BitRateIdx2_MASK                                       (0xF<<8)
#define     WAVBitRate_BitRateIdx1_e                                          7
#define     WAVBitRate_BitRateIdx1_SHIFT                                      4
#define     WAVBitRate_BitRateIdx1_MASK                                       (0xF<<4)
#define     WAVBitRate_BitRateIdx0_e                                          3
#define     WAVBitRate_BitRateIdx0_SHIFT                                      0
#define     WAVBitRate_BitRateIdx0_MASK                                       (0xF<<0)

#define     WAVTTimeH_TTimeH_e                                                5
#define     WAVTTimeH_TTimeH_SHIFT                                            0
#define     WAVTTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     WAVTTimeM_TTimeM_e                                                5
#define     WAVTTimeM_TTimeM_SHIFT                                            0
#define     WAVTTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     WAVTTimeS_TTimeS_e                                                5
#define     WAVTTimeS_TTimeS_SHIFT                                            0
#define     WAVTTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     WAVCTimeH_CTimeH_e                                                5
#define     WAVCTimeH_CTimeH_SHIFT                                            0
#define     WAVCTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     WAVCTimeM_CTimeM_e                                                5
#define     WAVCTimeM_CTimeM_SHIFT                                            0
#define     WAVCTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     WAVCTimeS_CTimeS_e                                                5
#define     WAVCTimeS_CTimeS_SHIFT                                            0
#define     WAVCTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     WAVBlockNum_BlockNum_e                                            23
#define     WAVBlockNum_BlockNum_SHIFT                                        0
#define     WAVBlockNum_BlockNum_MASK                                         (0xFFFFFF<<0)

#define     FLACIE_LackIntEn                                                  7
#define     FLACIE_EnergyIntEn                                                6
#define     FLACIE_WordIntEn                                                  5
#define     FLACIE_MetadataInfoEn                                             3
#define     FLACIE_ErrIntEn                                                   2
#define     FLACIE_InfoRdyIntEn                                               1
#define     FLACIE_PCMRdyIntEn                                                0

#define     FLACIP_LackIP                                                     7
#define     FLACIP_EnergyIP                                                   6
#define     FLACIP_WordIP                                                     5
#define     FLACIP_MetadataInfoIP                                             3
#define     FLACIP_ErrIP                                                      2
#define     FLACIP_InfoRdyIP                                                  1
#define     FLACIP_PCMRdyIP                                                   0

#define     FLACCtl_NewFile                                                   7
#define     FLACCtl_En                                                        6
#define     FLACCtl_FF_REV_En                                                 5
#define     FLACCtl_CRC16En                                                   0

#define     FLACHeaderInfo_BlockSize_e                                        15
#define     FLACHeaderInfo_BlockSize_SHIFT                                    12
#define     FLACHeaderInfo_BlockSize_MASK                                     (0xF<<12)
#define     FLACHeaderInfo_BlockingStrategy                                   11
#define     FLACHeaderInfo_SampleSize_e                                       10
#define     FLACHeaderInfo_SampleSize_SHIFT                                   8
#define     FLACHeaderInfo_SampleSize_MASK                                    (0x7<<8)
#define     FLACHeaderInfo_SampFrq_e                                          7
#define     FLACHeaderInfo_SampFrq_SHIFT                                      4
#define     FLACHeaderInfo_SampFrq_MASK                                       (0xF<<4)
#define     FLACHeaderInfo_Channel_assignment_e                               3
#define     FLACHeaderInfo_Channel_assignment_SHIFT                           0
#define     FLACHeaderInfo_Channel_assignment_MASK                            (0xF<<0)

#define     METADATAInfo_ID3V2                                                7
#define     METADATAInfo_PICTURE                                              6
#define     METADATAInfo_CUESHEET                                             5
#define     METADATAInfo_VORBIS                                               4
#define     METADATAInfo_SEEKTABLE                                            3
#define     METADATAInfo_APPLICATION                                          2
#define     METADATAInfo_PADDING                                              1
#define     METADATAInfo_STREAMINFO                                           0

#define     FLACTTimeH_TTimeH_e                                               5
#define     FLACTTimeH_TTimeH_SHIFT                                           0
#define     FLACTTimeH_TTimeH_MASK                                            (0x3F<<0)

#define     FLACTTimeM_TTimeM_e                                               5
#define     FLACTTimeM_TTimeM_SHIFT                                           0
#define     FLACTTimeM_TTimeM_MASK                                            (0x3F<<0)

#define     FLACTTimeS_TTimeS_e                                               5
#define     FLACTTimeS_TTimeS_SHIFT                                           0
#define     FLACTTimeS_TTimeS_MASK                                            (0x3F<<0)

#define     FLACCTimeH_CTimeH_e                                               5
#define     FLACCTimeH_CTimeH_SHIFT                                           0
#define     FLACCTimeH_CTimeH_MASK                                            (0x3F<<0)

#define     FLACCTimeM_CTimeM_e                                               5
#define     FLACCTimeM_CTimeM_SHIFT                                           0
#define     FLACCTimeM_CTimeM_MASK                                            (0x3F<<0)

#define     FLACCTimeS_CTimeS_e                                               5
#define     FLACCTimeS_CTimeS_SHIFT                                           0
#define     FLACCTimeS_CTimeS_MASK                                            (0x3F<<0)

#define     FLACErrInfo_CurrentSampleNum_bit35to32_e                          7
#define     FLACErrInfo_CurrentSampleNum_bit35to32_SHIFT                      4
#define     FLACErrInfo_CurrentSampleNum_bit35to32_MASK                       (0xF<<4)
#define     FLACErrInfo_ErrInfo_e                                             3
#define     FLACErrInfo_ErrInfo_SHIFT                                         0
#define     FLACErrInfo_ErrInfo_MASK                                          (0xF<<0)

#define     CurrentPos_CurrentPos_e                                           31
#define     CurrentPos_CurrentPos_SHIFT                                       0
#define     CurrentPos_CurrentPos_MASK                                        (0xFFFFFFFF<<0)

#define     FLACCFrameNum_CurrentSampleNum_bit31                              31
#define     FLACCFrameNum_CurrentFrame_bit30to0_e                             30
#define     FLACCFrameNum_CurrentFrame_bit30to0_SHIFT                         0
#define     FLACCFrameNum_CurrentFrame_bit30to0_MASK                          (0x7FFFFFFF<<0)

#define     FLACHeaderSynCtl_BlockingstrategyEn                               7
#define     FLACHeaderSynCtl_BlockSizeEn                                      6
#define     FLACHeaderSynCtl_SampleRateEn                                     5
#define     FLACHeaderSynCtl_ChannelNumEn                                     4
#define     FLACHeaderSynCtl_SampleSizeEn                                     3
#define     FLACHeaderSynCtl_SampleNumEn                                      2

#define     APEIE_LackIntEn                                                   7
#define     APEIE_EnergyIntEn                                                 6
#define     APEIE_WordIntEn                                                   5
#define     APEIE_ErrIntEn                                                    2
#define     APEIE_InfoRdyIntEn                                                1
#define     APEIE_PCMRdyIntEn                                                 0

#define     APEIP_LackIP                                                      7
#define     APEIP_EnergyIP                                                    6
#define     APEIP_WordIP                                                      5
#define     APEIP_ErrIP                                                       2
#define     APEIP_InfoRdyIP                                                   1
#define     APEIP_PCMRdyIP                                                    0

#define     APECtl_NewFile                                                    7
#define     APECtl_En                                                         6
#define     APECtl_SaturationPecision                                         5
#define     APECtl_PeakLevelEn                                                4
#define     APECtl_NewFrame                                                   3
#define     APECtl_SkipNum_e                                                  2
#define     APECtl_SkipNum_SHIFT                                              1
#define     APECtl_SkipNum_MASK                                               (0x3<<1)
#define     APECtl_CRC32En                                                    0

#define     APEHeaderInfo1_SampleRate_e                                       7
#define     APEHeaderInfo1_SampleRate_SHIFT                                   4
#define     APEHeaderInfo1_SampleRate_MASK                                    (0xF<<4)
#define     APEHeaderInfo1_nCompressionLevel_e                                3
#define     APEHeaderInfo1_nCompressionLevel_SHIFT                            1
#define     APEHeaderInfo1_nCompressionLevel_MASK                             (0x7<<1)

#define     APEHeaderInfo2_seek_element                                       4
#define     APEHeaderInfo2_nPeakLevel                                         3
#define     APEHeaderInfo2_SampleSize_e                                       2
#define     APEHeaderInfo2_SampleSize_SHIFT                                   1
#define     APEHeaderInfo2_SampleSize_MASK                                    (0x3<<1)
#define     APEHeaderInfo2_Channel_assignment                                 0

#define     APETTimeH_TTimeH_e                                                5
#define     APETTimeH_TTimeH_SHIFT                                            0
#define     APETTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     APETTimeM_TTimeM_e                                                5
#define     APETTimeM_TTimeM_SHIFT                                            0
#define     APETTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     APETTimeS_TTimeS_e                                                5
#define     APETTimeS_TTimeS_SHIFT                                            0
#define     APETTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     APECTimeH_CTimeH_e                                                5
#define     APECTimeH_CTimeH_SHIFT                                            0
#define     APECTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     APECTimeM_CTimeM_e                                                5
#define     APECTimeM_CTimeM_SHIFT                                            0
#define     APECTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     APECTimeS_CTimeS_e                                                5
#define     APECTimeS_CTimeS_SHIFT                                            0
#define     APECTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     APEErrInfo_ErrInfo_e                                              3
#define     APEErrInfo_ErrInfo_SHIFT                                          0
#define     APEErrInfo_ErrInfo_MASK                                           (0xF<<0)

#define     APECFrameNum_CurrentFrame_e                                       31
#define     APECFrameNum_CurrentFrame_SHIFT                                   0
#define     APECFrameNum_CurrentFrame_MASK                                    (0xFFFFFFFF<<0)

#define     SoftIE_LackIntEn                                                  7
#define     SoftIE_EnergyIntEn                                                6
#define     SoftIE_WordIntEn                                                  5
#define     SoftIE_FrameRdyIP                                                 3

#define     SoftIP_LackIP                                                     7
#define     SoftIP_EnergyIP                                                   6
#define     SoftIP_WordIP                                                     5
#define     SoftIP_FrameRdyIP                                                 3

#define     SoftCtl_NewFile                                                   7
#define     SoftCtl_En                                                        6
#define     SoftCtl_SampFrq_e                                                 5
#define     SoftCtl_SampFrq_SHIFT                                             2
#define     SoftCtl_SampFrq_MASK                                              (0xF<<2)
#define     SoftCtl_ChannelNum                                                1
#define     SoftCtl_NewFrame                                                  0

#define     FrameLen_FrameLen_e                                               11
#define     FrameLen_FrameLen_SHIFT                                           0
#define     FrameLen_FrameLen_MASK                                            (0xFFF<<0)

#define     BEPCtl1_EffectEn                                                  7
#define     BEPCtl1_EnergyEn                                                  6
#define     BEPCtl1_WhishtEn                                                  5
#define     BEPCtl1_FadeInEn                                                  4
#define     BEPCtl1_FadeOutEn                                                 3
#define     BEPCtl1_AutoFadeOutEn                                             2
#define     BEPCtl1_FadeOutP                                                  1
#define     BEPCtl1_Halt2_en                                                  0

#define     BEPCtl2_EqBandSel_e                                               7
#define     BEPCtl2_EqBandSel_SHIFT                                           4
#define     BEPCtl2_EqBandSel_MASK                                            (0xF<<4)
#define     BEPCtl2_FadeDuration_e                                            4
#define     BEPCtl2_FadeDuration_SHIFT                                        2
#define     BEPCtl2_FadeDuration_MASK                                         (0x7<<2)
#define     BEPCtl2_PCMFull                                                   1
#define     BEPCtl2_PCMEmpty                                                  0

#define     BEPCtl3_MuteEn                                                    7
#define     BEPCtl3_MuteL                                                     6
#define     BEPCtl3_MuteR                                                     5
#define     BEPCtl3_SilenceT_e                                                4
#define     BEPCtl3_SilenceT_SHIFT                                            0
#define     BEPCtl3_SilenceT_MASK                                             (0x1F<<0)

#define     BEPCtl4_WhishtSample_e                                            7
#define     BEPCtl4_WhishtSample_SHIFT                                        6
#define     BEPCtl4_WhishtSample_MASK                                         (0x3<<6)
#define     BEPCtl4_PCM_almost_full                                           5
#define     BEPCtl4_PCM_almost_empty                                          4
#define     BEPCtl4_EffectMode_e                                              3
#define     BEPCtl4_EffectMode_SHIFT                                          2
#define     BEPCtl4_EffectMode_MASK                                           (0x3<<2)
#define     BEPCtl4_Halt_en                                                   1
#define     BEPCtl4_FadeInActive                                              0

#define     BEPCtl5_EffectInBits                                              7
#define     BEPCtl5_EnergyIn_e                                                6
#define     BEPCtl5_EnergyIn_SHIFT                                            0
#define     BEPCtl5_EnergyIn_MASK                                             (0x7F<<0)

#define     GlobalGain_GlobalGain_e                                           23
#define     GlobalGain_GlobalGain_SHIFT                                       0
#define     GlobalGain_GlobalGain_MASK                                        (0xFFFFFF<<0)

#define     CurrentEnergy_Energy_e                                            7
#define     CurrentEnergy_Energy_SHIFT                                        0
#define     CurrentEnergy_Energy_MASK                                         (0xFF<<0)

#define     KaraokeCtl_MonoEn                                                 6
#define     KaraokeCtl_LRselect                                               5
#define     KaraokeCtl_LPF_e                                                  4
#define     KaraokeCtl_LPF_SHIFT                                              3
#define     KaraokeCtl_LPF_MASK                                               (0x3<<3)
#define     KaraokeCtl_HPF_e                                                  2
#define     KaraokeCtl_HPF_SHIFT                                              0
#define     KaraokeCtl_HPF_MASK                                               (0x7<<0)

#define     LCHGain_LCHgain_e                                                 7
#define     LCHGain_LCHgain_SHIFT                                             0
#define     LCHGain_LCHgain_MASK                                              (0xFF<<0)

#define     RCHGain_RCHgain_e                                                 7
#define     RCHGain_RCHgain_SHIFT                                             0
#define     RCHGain_RCHgain_MASK                                              (0xFF<<0)

#define     LPFGain_LPFgain_e                                                 7
#define     LPFGain_LPFgain_SHIFT                                             0
#define     LPFGain_LPFgain_MASK                                              (0xFF<<0)

#define     HPFGain_HPFgain_e                                                 7
#define     HPFGain_HPFgain_SHIFT                                             0
#define     HPFGain_HPFgain_MASK                                              (0xFF<<0)

#define     EQCoe0_SignSel                                                    6
#define     EQCoe0_GainCoefficient_e                                          5
#define     EQCoe0_GainCoefficient_SHIFT                                      0
#define     EQCoe0_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe1_SignSel                                                    6
#define     EQCoe1_GainCoefficient_e                                          5
#define     EQCoe1_GainCoefficient_SHIFT                                      0
#define     EQCoe1_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe2_SignSel                                                    6
#define     EQCoe2_GainCoefficient_e                                          5
#define     EQCoe2_GainCoefficient_SHIFT                                      0
#define     EQCoe2_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe3_SignSel                                                    6
#define     EQCoe3_GainCoefficient_e                                          5
#define     EQCoe3_GainCoefficient_SHIFT                                      0
#define     EQCoe3_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe4_SignSel                                                    6
#define     EQCoe4_GainCoefficient_e                                          5
#define     EQCoe4_GainCoefficient_SHIFT                                      0
#define     EQCoe4_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe5_SignSel                                                    6
#define     EQCoe5_GainCoefficient_e                                          5
#define     EQCoe5_GainCoefficient_SHIFT                                      0
#define     EQCoe5_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe6_SignSel                                                    6
#define     EQCoe6_GainCoefficient_e                                          5
#define     EQCoe6_GainCoefficient_SHIFT                                      0
#define     EQCoe6_GainCoefficient_MASK                                       (0x3F<<0)

#define     SRSCtl_LimEn                                                      0

#define     SRSCfg1_LfPeak_e                                                  2
#define     SRSCfg1_LfPeak_SHIFT                                              0
#define     SRSCfg1_LfPeak_MASK                                               (0x7<<0)

#define     SRSCfg2_CenterCtl_e                                               7
#define     SRSCfg2_CenterCtl_SHIFT                                           4
#define     SRSCfg2_CenterCtl_MASK                                            (0xF<<4)
#define     SRSCfg2_SpaceCtl_e                                                3
#define     SRSCfg2_SpaceCtl_SHIFT                                            0
#define     SRSCfg2_SpaceCtl_MASK                                             (0xF<<0)

#define     SRSCfg3_TrubassCtl_e                                              7
#define     SRSCfg3_TrubassCtl_SHIFT                                          4
#define     SRSCfg3_TrubassCtl_MASK                                           (0xF<<4)
#define     SRSCfg3_DefCtl_e                                                  3
#define     SRSCfg3_DefCtl_SHIFT                                              0
#define     SRSCfg3_DefCtl_MASK                                               (0xF<<0)

#define     SRSCfg4_FocusCtl_e                                                3
#define     SRSCfg4_FocusCtl_SHIFT                                            0
#define     SRSCfg4_FocusCtl_MASK                                             (0xF<<0)

#define     VPSCtl_CombineEn                                                  4
#define     VPSCtl_TimeScaleStage_e                                           3
#define     VPSCtl_TimeScaleStage_SHIFT                                       0
#define     VPSCtl_TimeScaleStage_MASK                                        (0xF<<0)

#define     SequenceSize_SequenceSize_e                                       11
#define     SequenceSize_SequenceSize_SHIFT                                   0
#define     SequenceSize_SequenceSize_MASK                                    (0xFFF<<0)

#define     OverlapSize_OverlapSize_e                                         8
#define     OverlapSize_OverlapSize_SHIFT                                     0
#define     OverlapSize_OverlapSize_MASK                                      (0x1FF<<0)

#define     SeekWindowSize_SeekWindowSize_e                                   8
#define     SeekWindowSize_SeekWindowSize_SHIFT                               0
#define     SeekWindowSize_SeekWindowSize_MASK                                (0x1FF<<0)

//--------------MJPEG-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     MJPEG_BASE                                                        0xc0140000
#define     Swreg0                                                            (MJPEG_BASE+0x0)
#define     Swreg1                                                            (MJPEG_BASE+0x4)
#define     Swreg2                                                            (MJPEG_BASE+0x8)
#define     Swreg3                                                            (MJPEG_BASE+0xC)
#define     Swreg4                                                            (MJPEG_BASE+0x10)
#define     Swreg5                                                            (MJPEG_BASE+0x14)
#define     Swreg6                                                            (MJPEG_BASE+0x18)
#define     Swreg7                                                            (MJPEG_BASE+0x1C)
#define     Swreg8                                                            (MJPEG_BASE+0x20)
#define     Swreg9                                                            (MJPEG_BASE+0x24)
#define     Swreg10                                                           (MJPEG_BASE+0x28)
#define     Swreg11                                                           (MJPEG_BASE+0x2C)
#define     Swreg12                                                           (MJPEG_BASE+0x30)
#define     Swreg13                                                           (MJPEG_BASE+0x34)
#define     Swreg14                                                           (MJPEG_BASE+0x38)
#define     Swreg15                                                           (MJPEG_BASE+0x3C)
#define     Swreg16                                                           (MJPEG_BASE+0x40)
#define     Swreg17                                                           (MJPEG_BASE+0x44)
#define     Swreg18                                                           (MJPEG_BASE+0x48)
#define     Swreg19                                                           (MJPEG_BASE+0x4C)
#define     Swreg20                                                           (MJPEG_BASE+0x50)
#define     Swreg21                                                           (MJPEG_BASE+0x54)
#define     Swreg22                                                           (MJPEG_BASE+0x58)
#define     Swreg23                                                           (MJPEG_BASE+0x5C)

//--------------Bits Location------------------------------------------//
#define     Swreg0_TimeIntEn                                                  2
#define     Swreg0_IntDis                                                     1
#define     Swreg0_Decoder_Start                                              0

#define     Swreg1_TimeOutIP                                                  5
#define     Swreg1_SliceRdyIP                                                 4
#define     Swreg1_StreamErrIP                                                3
#define     Swreg1_BufEmptyIP                                                 2
#define     Swreg1_FrameRdyIP                                                 1
#define     Swreg1_DecoderIntIP                                               0

#define     Swreg2_Width_MB_e                                                 31
#define     Swreg2_Width_MB_SHIFT                                             23
#define     Swreg2_Width_MB_MASK                                              (0x1FF<<23)
#define     Swreg2_Height_MB_e                                                22
#define     Swreg2_Height_MB_SHIFT                                            14
#define     Swreg2_Height_MB_MASK                                             (0x1FF<<14)
#define     Swreg2_UpscaleMode                                                8
#define     Swreg2_DownscaleMODE                                              7
#define     Swreg2_Amout_Qtab_e                                               6
#define     Swreg2_Amout_Qtab_SHIFT                                           5
#define     Swreg2_Amout_Qtab_MASK                                            (0x3<<5)
#define     Swreg2_JpegMODE_e                                                 4
#define     Swreg2_JpegMODE_SHIFT                                             2
#define     Swreg2_JpegMODE_MASK                                              (0x7<<2)
#define     Swreg2_PixelFill                                                  1
#define     Swreg2_JpegEnd                                                    0

#define     Swreg3_BitstreamOffset_e                                          23
#define     Swreg3_BitstreamOffset_SHIFT                                      15
#define     Swreg3_BitstreamOffset_MASK                                       (0x1FF<<15)
#define     Swreg3_StreamLen_e                                                14
#define     Swreg3_StreamLen_SHIFT                                            6
#define     Swreg3_StreamLen_MASK                                             (0x1FF<<6)
#define     Swreg3_StreamStrart_e                                             5
#define     Swreg3_StreamStrart_SHIFT                                         0
#define     Swreg3_StreamStrart_MASK                                          (0x3F<<0)

#define     Swreg4_VerDSampleRatio_e                                          5
#define     Swreg4_VerDSampleRatio_SHIFT                                      4
#define     Swreg4_VerDSampleRatio_MASK                                       (0x3<<4)
#define     Swreg4_VerDSampleEn                                               3
#define     Swreg4_HorDSampleRatio_e                                          2
#define     Swreg4_HorDSampleRatio_SHIFT                                      1
#define     Swreg4_HorDSampleRatio_MASK                                       (0x3<<1)
#define     Swreg4_HorDSampleEn                                               0

#define     Swreg5_VerScaleFac_e                                              31
#define     Swreg5_VerScaleFac_SHIFT                                          16
#define     Swreg5_VerScaleFac_MASK                                           (0xFFFF<<16)
#define     Swreg5_HorScaleFac_e                                              15
#define     Swreg5_HorScaleFac_SHIFT                                          0
#define     Swreg5_HorScaleFac_MASK                                           (0xFFFF<<0)

#define     Swreg6_SliceHeight_e                                              7
#define     Swreg6_SliceHeight_SHIFT                                          0
#define     Swreg6_SliceHeight_MASK                                           (0xFF<<0)

#define     Swreg7_OutputHeight_e                                             5
#define     Swreg7_OutputHeight_SHIFT                                         0
#define     Swreg7_OutputHeight_MASK                                          (0x3F<<0)

#define     Swreg8_DisHeight_e                                                16
#define     Swreg8_DisHeight_SHIFT                                            9
#define     Swreg8_DisHeight_MASK                                             (0xFF<<9)
#define     Swreg8_DisWidth_e                                                 8
#define     Swreg8_DisWidth_SHIFT                                             0
#define     Swreg8_DisWidth_MASK                                              (0x1FF<<0)

#define     Swreg9_Height_Pixel_e                                             25
#define     Swreg9_Height_Pixel_SHIFT                                         13
#define     Swreg9_Height_Pixel_MASK                                          (0x1FFF<<13)
#define     Swreg9_Width_Pixel_e                                              12
#define     Swreg9_Width_Pixel_SHIFT                                          0
#define     Swreg9_Width_Pixel_MASK                                           (0x1FFF<<0)

#define     Swreg10_CrACTab                                                   3
#define     Swreg10_CbACTab                                                   2
#define     Swreg10_CrDCTab                                                   1
#define     Swreg10_CbDCTab                                                   0

#define     Swreg11_ACTab1_Code6_e                                            30
#define     Swreg11_ACTab1_Code6_SHIFT                                        24
#define     Swreg11_ACTab1_Code6_MASK                                         (0x7F<<24)
#define     Swreg11_ACTab1_Code5_e                                            21
#define     Swreg11_ACTab1_Code5_SHIFT                                        16
#define     Swreg11_ACTab1_Code5_MASK                                         (0x3F<<16)
#define     Swreg11_ACTab1_Code4_e                                            15
#define     Swreg11_ACTab1_Code4_SHIFT                                        11
#define     Swreg11_ACTab1_Code4_MASK                                         (0x1F<<11)
#define     Swreg11_ACTab1_Code3_e                                            10
#define     Swreg11_ACTab1_Code3_SHIFT                                        7
#define     Swreg11_ACTab1_Code3_MASK                                         (0xF<<7)
#define     Swreg11_ACTab1_Code2_e                                            5
#define     Swreg11_ACTab1_Code2_SHIFT                                        3
#define     Swreg11_ACTab1_Code2_MASK                                         (0x7<<3)
#define     Swreg11_ACTab1_Code1_e                                            1
#define     Swreg11_ACTab1_Code1_SHIFT                                        0
#define     Swreg11_ACTab1_Code1_MASK                                         (0x3<<0)

#define     Swreg12_ACTab1_Code10_e                                           31
#define     Swreg12_ACTab1_Code10_SHIFT                                       24
#define     Swreg12_ACTab1_Code10_MASK                                        (0xFF<<24)
#define     Swreg12_ACTab1_Code9_e                                            23
#define     Swreg12_ACTab1_Code9_SHIFT                                        16
#define     Swreg12_ACTab1_Code9_MASK                                         (0xFF<<16)
#define     Swreg12_ACTab1_Code8_e                                            15
#define     Swreg12_ACTab1_Code8_SHIFT                                        8
#define     Swreg12_ACTab1_Code8_MASK                                         (0xFF<<8)
#define     Swreg12_ACTab1_Code7_e                                            7
#define     Swreg12_ACTab1_Code7_SHIFT                                        0
#define     Swreg12_ACTab1_Code7_MASK                                         (0xFF<<0)

#define     Swreg13_ACTab1_Code14_e                                           31
#define     Swreg13_ACTab1_Code14_SHIFT                                       24
#define     Swreg13_ACTab1_Code14_MASK                                        (0xFF<<24)
#define     Swreg13_ACTab1_Code13_e                                           23
#define     Swreg13_ACTab1_Code13_SHIFT                                       16
#define     Swreg13_ACTab1_Code13_MASK                                        (0xFF<<16)
#define     Swreg13_ACTab1_Code12_e                                           15
#define     Swreg13_ACTab1_Code12_SHIFT                                       8
#define     Swreg13_ACTab1_Code12_MASK                                        (0xFF<<8)
#define     Swreg13_ACTab1_Code11_e                                           7
#define     Swreg13_ACTab1_Code11_SHIFT                                       0
#define     Swreg13_ACTab1_Code11_MASK                                        (0xFF<<0)

#define     Swreg14_ACTab2_Code4_e                                            31
#define     Swreg14_ACTab2_Code4_SHIFT                                        27
#define     Swreg14_ACTab2_Code4_MASK                                         (0x1F<<27)
#define     Swreg14_ACTab2_Code3_e                                            26
#define     Swreg14_ACTab2_Code3_SHIFT                                        23
#define     Swreg14_ACTab2_Code3_MASK                                         (0xF<<23)
#define     Swreg14_ACTab2_Code2_e                                            21
#define     Swreg14_ACTab2_Code2_SHIFT                                        19
#define     Swreg14_ACTab2_Code2_MASK                                         (0x7<<19)
#define     Swreg14_ACTab2_Code1_e                                            17
#define     Swreg14_ACTab2_Code1_SHIFT                                        16
#define     Swreg14_ACTab2_Code1_MASK                                         (0x3<<16)
#define     Swreg14_ACTab1_Code16_e                                           15
#define     Swreg14_ACTab1_Code16_SHIFT                                       8
#define     Swreg14_ACTab1_Code16_MASK                                        (0xFF<<8)
#define     Swreg14_ACTab1_Code15_e                                           7
#define     Swreg14_ACTab1_Code15_SHIFT                                       0
#define     Swreg14_ACTab1_Code15_MASK                                        (0xFF<<0)

#define     Swreg15_ACTab2_Code8_e                                            31
#define     Swreg15_ACTab2_Code8_SHIFT                                        24
#define     Swreg15_ACTab2_Code8_MASK                                         (0xFF<<24)
#define     Swreg15_ACTab2_Code7_e                                            23
#define     Swreg15_ACTab2_Code7_SHIFT                                        16
#define     Swreg15_ACTab2_Code7_MASK                                         (0xFF<<16)
#define     Swreg15_ACTab2_Code6_e                                            14
#define     Swreg15_ACTab2_Code6_SHIFT                                        8
#define     Swreg15_ACTab2_Code6_MASK                                         (0x7F<<8)
#define     Swreg15_ACTab2_Code5_e                                            5
#define     Swreg15_ACTab2_Code5_SHIFT                                        0
#define     Swreg15_ACTab2_Code5_MASK                                         (0x3F<<0)

#define     Swreg16_ACTab2_Code12_e                                           31
#define     Swreg16_ACTab2_Code12_SHIFT                                       24
#define     Swreg16_ACTab2_Code12_MASK                                        (0xFF<<24)
#define     Swreg16_ACTab2_Code11_e                                           23
#define     Swreg16_ACTab2_Code11_SHIFT                                       16
#define     Swreg16_ACTab2_Code11_MASK                                        (0xFF<<16)
#define     Swreg16_ACTab2_Code10_e                                           15
#define     Swreg16_ACTab2_Code10_SHIFT                                       8
#define     Swreg16_ACTab2_Code10_MASK                                        (0xFF<<8)
#define     Swreg16_ACTab2_Code9_e                                            7
#define     Swreg16_ACTab2_Code9_SHIFT                                        0
#define     Swreg16_ACTab2_Code9_MASK                                         (0xFF<<0)

#define     Swreg17_ACTab2_Code16_e                                           31
#define     Swreg17_ACTab2_Code16_SHIFT                                       24
#define     Swreg17_ACTab2_Code16_MASK                                        (0xFF<<24)
#define     Swreg17_ACTab2_Code15_e                                           23
#define     Swreg17_ACTab2_Code15_SHIFT                                       16
#define     Swreg17_ACTab2_Code15_MASK                                        (0xFF<<16)
#define     Swreg17_ACTab2_Code14_e                                           15
#define     Swreg17_ACTab2_Code14_SHIFT                                       8
#define     Swreg17_ACTab2_Code14_MASK                                        (0xFF<<8)
#define     Swreg17_ACTab2_Code13_e                                           7
#define     Swreg17_ACTab2_Code13_SHIFT                                       0
#define     Swreg17_ACTab2_Code13_MASK                                        (0xFF<<0)

#define     Swreg18_DCTab1_Code8_e                                            31
#define     Swreg18_DCTab1_Code8_SHIFT                                        28
#define     Swreg18_DCTab1_Code8_MASK                                         (0xF<<28)
#define     Swreg18_DCTab1_Code7_e                                            27
#define     Swreg18_DCTab1_Code7_SHIFT                                        24
#define     Swreg18_DCTab1_Code7_MASK                                         (0xF<<24)
#define     Swreg18_DCTab1_Code6_e                                            23
#define     Swreg18_DCTab1_Code6_SHIFT                                        20
#define     Swreg18_DCTab1_Code6_MASK                                         (0xF<<20)
#define     Swreg18_DCTab1_Code5_e                                            19
#define     Swreg18_DCTab1_Code5_SHIFT                                        16
#define     Swreg18_DCTab1_Code5_MASK                                         (0xF<<16)
#define     Swreg18_DCTab1_Code4_e                                            15
#define     Swreg18_DCTab1_Code4_SHIFT                                        12
#define     Swreg18_DCTab1_Code4_MASK                                         (0xF<<12)
#define     Swreg18_DCTab1_Code3_e                                            11
#define     Swreg18_DCTab1_Code3_SHIFT                                        8
#define     Swreg18_DCTab1_Code3_MASK                                         (0xF<<8)
#define     Swreg18_DCTab1_Code2_e                                            6
#define     Swreg18_DCTab1_Code2_SHIFT                                        4
#define     Swreg18_DCTab1_Code2_MASK                                         (0x7<<4)
#define     Swreg18_DCTab1_Code1_e                                            1
#define     Swreg18_DCTab1_Code1_SHIFT                                        0
#define     Swreg18_DCTab1_Code1_MASK                                         (0x3<<0)

#define     Swreg19_DCTab1_Code16_e                                           31
#define     Swreg19_DCTab1_Code16_SHIFT                                       28
#define     Swreg19_DCTab1_Code16_MASK                                        (0xF<<28)
#define     Swreg19_DCTab1_Code15_e                                           27
#define     Swreg19_DCTab1_Code15_SHIFT                                       24
#define     Swreg19_DCTab1_Code15_MASK                                        (0xF<<24)
#define     Swreg19_DCTab1_Code14_e                                           23
#define     Swreg19_DCTab1_Code14_SHIFT                                       20
#define     Swreg19_DCTab1_Code14_MASK                                        (0xF<<20)
#define     Swreg19_DCTab1_Code13_e                                           19
#define     Swreg19_DCTab1_Code13_SHIFT                                       16
#define     Swreg19_DCTab1_Code13_MASK                                        (0xF<<16)
#define     Swreg19_DCTab1_Code12_e                                           15
#define     Swreg19_DCTab1_Code12_SHIFT                                       12
#define     Swreg19_DCTab1_Code12_MASK                                        (0xF<<12)
#define     Swreg19_DCTab1_Code11_e                                           11
#define     Swreg19_DCTab1_Code11_SHIFT                                       8
#define     Swreg19_DCTab1_Code11_MASK                                        (0xF<<8)
#define     Swreg19_DCTab1_Code10_e                                           7
#define     Swreg19_DCTab1_Code10_SHIFT                                       4
#define     Swreg19_DCTab1_Code10_MASK                                        (0xF<<4)
#define     Swreg19_DCTab1_Code9_e                                            3
#define     Swreg19_DCTab1_Code9_SHIFT                                        0
#define     Swreg19_DCTab1_Code9_MASK                                         (0xF<<0)

#define     Swreg20_DCTab2_Code8_e                                            31
#define     Swreg20_DCTab2_Code8_SHIFT                                        28
#define     Swreg20_DCTab2_Code8_MASK                                         (0xF<<28)
#define     Swreg20_DCTab2_Code7_e                                            27
#define     Swreg20_DCTab2_Code7_SHIFT                                        24
#define     Swreg20_DCTab2_Code7_MASK                                         (0xF<<24)
#define     Swreg20_DCTab2_Code6_e                                            23
#define     Swreg20_DCTab2_Code6_SHIFT                                        20
#define     Swreg20_DCTab2_Code6_MASK                                         (0xF<<20)
#define     Swreg20_DCTab2_Code5_e                                            19
#define     Swreg20_DCTab2_Code5_SHIFT                                        16
#define     Swreg20_DCTab2_Code5_MASK                                         (0xF<<16)
#define     Swreg20_DCTab2_Code4_e                                            15
#define     Swreg20_DCTab2_Code4_SHIFT                                        12
#define     Swreg20_DCTab2_Code4_MASK                                         (0xF<<12)
#define     Swreg20_DCTab2_Code3_e                                            11
#define     Swreg20_DCTab2_Code3_SHIFT                                        8
#define     Swreg20_DCTab2_Code3_MASK                                         (0xF<<8)
#define     Swreg20_DCTab2_Code2_e                                            6
#define     Swreg20_DCTab2_Code2_SHIFT                                        4
#define     Swreg20_DCTab2_Code2_MASK                                         (0x7<<4)
#define     Swreg20_DCTab2_Code1_e                                            1
#define     Swreg20_DCTab2_Code1_SHIFT                                        0
#define     Swreg20_DCTab2_Code1_MASK                                         (0x3<<0)

#define     Swreg21_DCTab2_Code16_e                                           31
#define     Swreg21_DCTab2_Code16_SHIFT                                       28
#define     Swreg21_DCTab2_Code16_MASK                                        (0xF<<28)
#define     Swreg21_DCTab2_Code15_e                                           27
#define     Swreg21_DCTab2_Code15_SHIFT                                       24
#define     Swreg21_DCTab2_Code15_MASK                                        (0xF<<24)
#define     Swreg21_DCTab2_Code14_e                                           23
#define     Swreg21_DCTab2_Code14_SHIFT                                       20
#define     Swreg21_DCTab2_Code14_MASK                                        (0xF<<20)
#define     Swreg21_DCTab2_Code13_e                                           19
#define     Swreg21_DCTab2_Code13_SHIFT                                       16
#define     Swreg21_DCTab2_Code13_MASK                                        (0xF<<16)
#define     Swreg21_DCTab2_Code12_e                                           15
#define     Swreg21_DCTab2_Code12_SHIFT                                       12
#define     Swreg21_DCTab2_Code12_MASK                                        (0xF<<12)
#define     Swreg21_DCTab2_Code11_e                                           11
#define     Swreg21_DCTab2_Code11_SHIFT                                       8
#define     Swreg21_DCTab2_Code11_MASK                                        (0xF<<8)
#define     Swreg21_DCTab2_Code10_e                                           7
#define     Swreg21_DCTab2_Code10_SHIFT                                       4
#define     Swreg21_DCTab2_Code10_MASK                                        (0xF<<4)
#define     Swreg21_DCTab2_Code9_e                                            3
#define     Swreg21_DCTab2_Code9_SHIFT                                        0
#define     Swreg21_DCTab2_Code9_MASK                                         (0xF<<0)

#define     Swreg22_OutModeSel                                                3
#define     Swreg22_Jram67Map                                                 2
#define     Swreg22_Jram58Map                                                 1
#define     Swreg22_Jram01Map                                                 0

#define     Swreg23_DebugOutSel_e                                             7
#define     Swreg23_DebugOutSel_SHIFT                                         5
#define     Swreg23_DebugOutSel_MASK                                          (0x7<<5)
#define     Swreg23_ResultSelBit_e                                            4
#define     Swreg23_ResultSelBit_SHIFT                                        3
#define     Swreg23_ResultSelBit_MASK                                         (0x3<<3)
#define     Swreg23_MidResultSel_e                                            2
#define     Swreg23_MidResultSel_SHIFT                                        1
#define     Swreg23_MidResultSel_MASK                                         (0x3<<1)
#define     Swreg23_DebugEn                                                   0

//--------------NAND_Control_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     NAND_Control_Register_BASE                                        0xC0070000
#define     NAND_CTL                                                          (NAND_Control_Register_BASE+0x0000)
#define     NAND_STATUS                                                       (NAND_Control_Register_BASE+0X0004)
#define     NAND_CONFIG                                                       (NAND_Control_Register_BASE+0X0008)
#define     NAND_BC                                                           (NAND_Control_Register_BASE+0X000c)
#define     NAND_DATA                                                         (NAND_Control_Register_BASE+0X0010)
#define     NAND_COLADDR                                                      (NAND_Control_Register_BASE+0X0014)
#define     NAND_ROWADDR0                                                     (NAND_Control_Register_BASE+0X0018)
#define     NAND_ROWADDR1                                                     (NAND_Control_Register_BASE+0X001c)
#define     NAND_CMD_FSM0                                                     (NAND_Control_Register_BASE+0X0020)
#define     NAND_CMD_FSM_CTL0                                                 (NAND_Control_Register_BASE+0x0024)
#define     NAND_CMD_FSM1                                                     (NAND_Control_Register_BASE+0X0028)
#define     NAND_CMD_FSM_CTL1                                                 (NAND_Control_Register_BASE+0x002c)
#define     NAND_CMD_FSM2                                                     (NAND_Control_Register_BASE+0X0030)
#define     NAND_CMD_FSM_CTL2                                                 (NAND_Control_Register_BASE+0x0034)
#define     NAND_FSM_START                                                    (NAND_Control_Register_BASE+0x0038)
#define     NAND_ECCST0                                                       (NAND_Control_Register_BASE+0X003c)
#define     NAND_ECCST1                                                       (NAND_Control_Register_BASE+0X0040)
#define     NAND_ECCST2                                                       (NAND_Control_Register_BASE+0X0044)
#define     NAND_ECCST3                                                       (NAND_Control_Register_BASE+0X0048)
#define     NAND_UDATA0                                                       (NAND_Control_Register_BASE+0X004c)
#define     NAND_UDATA1                                                       (NAND_Control_Register_BASE+0X0050)
#define     NAND_UDATA2                                                       (NAND_Control_Register_BASE+0X0054)
#define     NAND_UDATA3                                                       (NAND_Control_Register_BASE+0X0058)
#define     NAND_TIMING                                                       (NAND_Control_Register_BASE+0X005c)

//--------------Bits Location------------------------------------------//
#define     NAND_CTL_URMEN                                                    31
#define     NAND_CTL_DL_e                                                     30
#define     NAND_CTL_DL_SHIFT                                                 28
#define     NAND_CTL_DL_MASK                                                  (0x7<<28)
#define     NAND_CTL_RBIE                                                     20
#define     NAND_CTL_EIE                                                      19
#define     NAND_CTL_IFS_e                                                    13
#define     NAND_CTL_IFS_SHIFT                                                12
#define     NAND_CTL_IFS_MASK                                                 (0x3<<12)
#define     NAND_CTL_BSEL                                                     8
#define     NAND_CTL_CE3E                                                     6
#define     NAND_CTL_CE2E                                                     5
#define     NAND_CTL_CE1E                                                     4
#define     NAND_CTL_CE0E                                                     3
#define     NAND_CTL_CECT                                                     2
#define     NAND_CTL_CKCTL                                                    1
#define     NAND_CTL_EN                                                       0

#define     NAND_STATUS_FSMS                                                  31
#define     NAND_STATUS_FSMS0                                                 30
#define     NAND_STATUS_FSMS1                                                 29
#define     NAND_STATUS_FSMS2                                                 28
#define     NAND_STATUS_ECCS_e                                                21
#define     NAND_STATUS_ECCS_SHIFT                                            16
#define     NAND_STATUS_ECCS_MASK                                             (0x3F<<16)
#define     NAND_STATUS_TMSN_e                                                15
#define     NAND_STATUS_TMSN_SHIFT                                            12
#define     NAND_STATUS_TMSN_MASK                                             (0xF<<12)
#define     NAND_STATUS_DAFF_e                                                11
#define     NAND_STATUS_DAFF_SHIFT                                            8
#define     NAND_STATUS_DAFF_MASK                                             (0xF<<8)
#define     NAND_STATUS_RDRQ                                                  4
#define     NAND_STATUS_RBST                                                  3
#define     NAND_STATUS_RBPD                                                  1
#define     NAND_STATUS_EIP                                                   0

#define     NAND_CONFIG_CHIPIDIN_e                                            21
#define     NAND_CONFIG_CHIPIDIN_SHIFT                                        16
#define     NAND_CONFIG_CHIPIDIN_MASK                                         (0x3F<<16)
#define     NAND_CONFIG_UDSEL_e                                               9
#define     NAND_CONFIG_UDSEL_SHIFT                                           8
#define     NAND_CONFIG_UDSEL_MASK                                            (0x3<<8)
#define     NAND_CONFIG_PPS                                                   7
#define     NAND_CONFIG_ROWADD_e                                              6
#define     NAND_CONFIG_ROWADD_SHIFT                                          4
#define     NAND_CONFIG_ROWADD_MASK                                           (0x7<<4)
#define     NAND_CONFIG_ECCTYP_e                                              1
#define     NAND_CONFIG_ECCTYP_SHIFT                                          0
#define     NAND_CONFIG_ECCTYP_MASK                                           (0x3<<0)

#define     NAND_BC_MBYTECNT_e                                                10
#define     NAND_BC_MBYTECNT_SHIFT                                            0
#define     NAND_BC_MBYTECNT_MASK                                             (0x7FF<<0)

#define     NAND_DATA_DATA_e                                                  31
#define     NAND_DATA_DATA_SHIFT                                              0
#define     NAND_DATA_DATA_MASK                                               (0xFFFFFFFF<<0)

#define     NAND_COLADDR_SPCOLL1_e                                            31
#define     NAND_COLADDR_SPCOLL1_SHIFT                                        24
#define     NAND_COLADDR_SPCOLL1_MASK                                         (0xFF<<24)
#define     NAND_COLADDR_SPCOLL0_e                                            23
#define     NAND_COLADDR_SPCOLL0_SHIFT                                        16
#define     NAND_COLADDR_SPCOLL0_MASK                                         (0xFF<<16)
#define     NAND_COLADDR_MCOLL1_e                                             15
#define     NAND_COLADDR_MCOLL1_SHIFT                                         8
#define     NAND_COLADDR_MCOLL1_MASK                                          (0xFF<<8)
#define     NAND_COLADDR_MCOLL0_e                                             7
#define     NAND_COLADDR_MCOLL0_SHIFT                                         0
#define     NAND_COLADDR_MCOLL0_MASK                                          (0xFF<<0)

#define     NAND_ROWADDR0_ROWLOW3_e                                           31
#define     NAND_ROWADDR0_ROWLOW3_SHIFT                                       24
#define     NAND_ROWADDR0_ROWLOW3_MASK                                        (0xFF<<24)
#define     NAND_ROWADDR0_ROWLOW2_e                                           23
#define     NAND_ROWADDR0_ROWLOW2_SHIFT                                       16
#define     NAND_ROWADDR0_ROWLOW2_MASK                                        (0xFF<<16)
#define     NAND_ROWADDR0_ROWLOW1_e                                           15
#define     NAND_ROWADDR0_ROWLOW1_SHIFT                                       8
#define     NAND_ROWADDR0_ROWLOW1_MASK                                        (0xFF<<8)
#define     NAND_ROWADDR0_ROWLOW0_e                                           7
#define     NAND_ROWADDR0_ROWLOW0_SHIFT                                       0
#define     NAND_ROWADDR0_ROWLOW0_MASK                                        (0xFF<<0)

#define     NAND_ROWADDR1_ROWLOW4_e                                           7
#define     NAND_ROWADDR1_ROWLOW4_SHIFT                                       0
#define     NAND_ROWADDR1_ROWLOW4_MASK                                        (0xFF<<0)

#define     NAND_CMD_FSM0_CMD3_e                                              31
#define     NAND_CMD_FSM0_CMD3_SHIFT                                          24
#define     NAND_CMD_FSM0_CMD3_MASK                                           (0xFF<<24)
#define     NAND_CMD_FSM0_CMD2_e                                              23
#define     NAND_CMD_FSM0_CMD2_SHIFT                                          16
#define     NAND_CMD_FSM0_CMD2_MASK                                           (0xFF<<16)
#define     NAND_CMD_FSM0_CMD1_e                                              15
#define     NAND_CMD_FSM0_CMD1_SHIFT                                          8
#define     NAND_CMD_FSM0_CMD1_MASK                                           (0xFF<<8)
#define     NAND_CMD_FSM0_CMD0_e                                              7
#define     NAND_CMD_FSM0_CMD0_SHIFT                                          0
#define     NAND_CMD_FSM0_CMD0_MASK                                           (0xFF<<0)

#define     NAND_CMD_FSM_CTL0_FSMENC3                                         30
#define     NAND_CMD_FSM_CTL0_SCMD3                                           29
#define     NAND_CMD_FSM_CTL0_ACC3                                            28
#define     NAND_CMD_FSM_CTL0_SROW3                                           27
#define     NAND_CMD_FSM_CTL0_SCOL3                                           26
#define     NAND_CMD_FSM_CTL0_RBDAT3                                          25
#define     NAND_CMD_FSM_CTL0_DATATS3                                         24
#define     NAND_CMD_FSM_CTL0_FSMENC2                                         22
#define     NAND_CMD_FSM_CTL0_SCMD2                                           21
#define     NAND_CMD_FSM_CTL0_ACC2                                            20
#define     NAND_CMD_FSM_CTL0_SROW2                                           19
#define     NAND_CMD_FSM_CTL0_SCOL2                                           18
#define     NAND_CMD_FSM_CTL0_RBDAT2                                          17
#define     NAND_CMD_FSM_CTL0_DATATS2                                         16
#define     NAND_CMD_FSM_CTL0_FSMENC1                                         14
#define     NAND_CMD_FSM_CTL0_SCMD1                                           13
#define     NAND_CMD_FSM_CTL0_ACC1                                            12
#define     NAND_CMD_FSM_CTL0_SROW1                                           11
#define     NAND_CMD_FSM_CTL0_SCOL1                                           10
#define     NAND_CMD_FSM_CTL0_RBDAT1                                          9
#define     NAND_CMD_FSM_CTL0_DATATS1                                         8
#define     NAND_CMD_FSM_CTL0_FSMENC0                                         6
#define     NAND_CMD_FSM_CTL0_SCMD0                                           5
#define     NAND_CMD_FSM_CTL0_ACC0                                            4
#define     NAND_CMD_FSM_CTL0_SROW0                                           3
#define     NAND_CMD_FSM_CTL0_SCOL0                                           2
#define     NAND_CMD_FSM_CTL0_RBDAT0                                          1
#define     NAND_CMD_FSM_CTL0_DATATS0                                         0

#define     NAND_CMD_FSM1_CMD3_e                                              31
#define     NAND_CMD_FSM1_CMD3_SHIFT                                          24
#define     NAND_CMD_FSM1_CMD3_MASK                                           (0xFF<<24)
#define     NAND_CMD_FSM1_CMD2_e                                              23
#define     NAND_CMD_FSM1_CMD2_SHIFT                                          16
#define     NAND_CMD_FSM1_CMD2_MASK                                           (0xFF<<16)
#define     NAND_CMD_FSM1_CMD1_e                                              15
#define     NAND_CMD_FSM1_CMD1_SHIFT                                          8
#define     NAND_CMD_FSM1_CMD1_MASK                                           (0xFF<<8)
#define     NAND_CMD_FSM1_CMD0_e                                              7
#define     NAND_CMD_FSM1_CMD0_SHIFT                                          0
#define     NAND_CMD_FSM1_CMD0_MASK                                           (0xFF<<0)

#define     NAND_CMD_FSM_CTL1_FSMENC3                                         30
#define     NAND_CMD_FSM_CTL1_SCMD3                                           29
#define     NAND_CMD_FSM_CTL1_ACC3                                            28
#define     NAND_CMD_FSM_CTL1_SROW3                                           27
#define     NAND_CMD_FSM_CTL1_SCOL3                                           26
#define     NAND_CMD_FSM_CTL1_RBDAT3                                          25
#define     NAND_CMD_FSM_CTL1_DATATS3                                         24
#define     NAND_CMD_FSM_CTL1_FSMENC2                                         22
#define     NAND_CMD_FSM_CTL1_SCMD2                                           21
#define     NAND_CMD_FSM_CTL1_ACC2                                            20
#define     NAND_CMD_FSM_CTL1_SROW2                                           19
#define     NAND_CMD_FSM_CTL1_SCOL2                                           18
#define     NAND_CMD_FSM_CTL1_RBDAT2                                          17
#define     NAND_CMD_FSM_CTL1_DATATS2                                         16
#define     NAND_CMD_FSM_CTL1_FSMENC1                                         14
#define     NAND_CMD_FSM_CTL1_SCMD1                                           13
#define     NAND_CMD_FSM_CTL1_ACC1                                            12
#define     NAND_CMD_FSM_CTL1_SROW1                                           11
#define     NAND_CMD_FSM_CTL1_SCOL1                                           10
#define     NAND_CMD_FSM_CTL1_RBDAT1                                          9
#define     NAND_CMD_FSM_CTL1_DATATS1                                         8
#define     NAND_CMD_FSM_CTL1_FSMENC0                                         6
#define     NAND_CMD_FSM_CTL1_SCMD0                                           5
#define     NAND_CMD_FSM_CTL1_ACC0                                            4
#define     NAND_CMD_FSM_CTL1_SROW0                                           3
#define     NAND_CMD_FSM_CTL1_SCOL0                                           2
#define     NAND_CMD_FSM_CTL1_RBDAT0                                          1
#define     NAND_CMD_FSM_CTL1_DATATS0                                         0

#define     NAND_CMD_FSM2_CMD3_e                                              31
#define     NAND_CMD_FSM2_CMD3_SHIFT                                          24
#define     NAND_CMD_FSM2_CMD3_MASK                                           (0xFF<<24)
#define     NAND_CMD_FSM2_CMD2_e                                              23
#define     NAND_CMD_FSM2_CMD2_SHIFT                                          16
#define     NAND_CMD_FSM2_CMD2_MASK                                           (0xFF<<16)
#define     NAND_CMD_FSM2_CMD1_e                                              15
#define     NAND_CMD_FSM2_CMD1_SHIFT                                          8
#define     NAND_CMD_FSM2_CMD1_MASK                                           (0xFF<<8)
#define     NAND_CMD_FSM2_CMD0_e                                              7
#define     NAND_CMD_FSM2_CMD0_SHIFT                                          0
#define     NAND_CMD_FSM2_CMD0_MASK                                           (0xFF<<0)

#define     NAND_CMD_FSM_CTL2_FSMENC3                                         30
#define     NAND_CMD_FSM_CTL2_SCMD3                                           29
#define     NAND_CMD_FSM_CTL2_ACC3                                            28
#define     NAND_CMD_FSM_CTL2_SROW3                                           27
#define     NAND_CMD_FSM_CTL2_SCOL3                                           26
#define     NAND_CMD_FSM_CTL2_RBDAT3                                          25
#define     NAND_CMD_FSM_CTL2_DATATS3                                         24
#define     NAND_CMD_FSM_CTL2_FSMENC2                                         22
#define     NAND_CMD_FSM_CTL2_SCMD2                                           21
#define     NAND_CMD_FSM_CTL2_ACC2                                            20
#define     NAND_CMD_FSM_CTL2_SROW2                                           19
#define     NAND_CMD_FSM_CTL2_SCOL2                                           18
#define     NAND_CMD_FSM_CTL2_RBDAT2                                          17
#define     NAND_CMD_FSM_CTL2_DATATS2                                         16
#define     NAND_CMD_FSM_CTL2_FSMENC1                                         14
#define     NAND_CMD_FSM_CTL2_SCMD1                                           13
#define     NAND_CMD_FSM_CTL2_ACC1                                            12
#define     NAND_CMD_FSM_CTL2_SROW1                                           11
#define     NAND_CMD_FSM_CTL2_SCOL1                                           10
#define     NAND_CMD_FSM_CTL2_RBDAT1                                          9
#define     NAND_CMD_FSM_CTL2_DATATS1                                         8
#define     NAND_CMD_FSM_CTL2_FSMENC0                                         6
#define     NAND_CMD_FSM_CTL2_SCMD0                                           5
#define     NAND_CMD_FSM_CTL2_ACC0                                            4
#define     NAND_CMD_FSM_CTL2_SROW0                                           3
#define     NAND_CMD_FSM_CTL2_SCOL0                                           2
#define     NAND_CMD_FSM_CTL2_RBDAT0                                          1
#define     NAND_CMD_FSM_CTL2_DATATS0                                         0

#define     NAND_FSM_START_DQSLEN                                             12
#define     NAND_FSM_START_FSM1CTLN_e                                         11
#define     NAND_FSM_START_FSM1CTLN_SHIFT                                     8
#define     NAND_FSM_START_FSM1CTLN_MASK                                      (0xF<<8)
#define     NAND_FSM_START_EACC                                               7
#define     NAND_FSM_START_CHIPIDEN                                           5
#define     NAND_FSM_START_RANEN_e                                            4
#define     NAND_FSM_START_RANEN_SHIFT                                        3
#define     NAND_FSM_START_RANEN_MASK                                         (0x3<<3)
#define     NAND_FSM_START_ECCEN                                              2
#define     NAND_FSM_START_RWDI                                               1
#define     NAND_FSM_START_FSMST                                              0

#define     NAND_ECCST0_ECCST3_e                                              29
#define     NAND_ECCST0_ECCST3_SHIFT                                          24
#define     NAND_ECCST0_ECCST3_MASK                                           (0x3F<<24)
#define     NAND_ECCST0_ECCST2_e                                              21
#define     NAND_ECCST0_ECCST2_SHIFT                                          16
#define     NAND_ECCST0_ECCST2_MASK                                           (0x3F<<16)
#define     NAND_ECCST0_ECCST1_e                                              13
#define     NAND_ECCST0_ECCST1_SHIFT                                          8
#define     NAND_ECCST0_ECCST1_MASK                                           (0x3F<<8)
#define     NAND_ECCST0_ECCST0_e                                              5
#define     NAND_ECCST0_ECCST0_SHIFT                                          0
#define     NAND_ECCST0_ECCST0_MASK                                           (0x3F<<0)

#define     NAND_ECCST1_ECCST7_e                                              29
#define     NAND_ECCST1_ECCST7_SHIFT                                          24
#define     NAND_ECCST1_ECCST7_MASK                                           (0x3F<<24)
#define     NAND_ECCST1_ECCST6_e                                              21
#define     NAND_ECCST1_ECCST6_SHIFT                                          16
#define     NAND_ECCST1_ECCST6_MASK                                           (0x3F<<16)
#define     NAND_ECCST1_ECCST5_e                                              13
#define     NAND_ECCST1_ECCST5_SHIFT                                          8
#define     NAND_ECCST1_ECCST5_MASK                                           (0x3F<<8)
#define     NAND_ECCST1_ECCST4_e                                              5
#define     NAND_ECCST1_ECCST4_SHIFT                                          0
#define     NAND_ECCST1_ECCST4_MASK                                           (0x3F<<0)

#define     NAND_ECCST2_ECCSTb_e                                              29
#define     NAND_ECCST2_ECCSTb_SHIFT                                          24
#define     NAND_ECCST2_ECCSTb_MASK                                           (0x3F<<24)
#define     NAND_ECCST2_ECCSTa_e                                              21
#define     NAND_ECCST2_ECCSTa_SHIFT                                          16
#define     NAND_ECCST2_ECCSTa_MASK                                           (0x3F<<16)
#define     NAND_ECCST2_ECCST9_e                                              13
#define     NAND_ECCST2_ECCST9_SHIFT                                          8
#define     NAND_ECCST2_ECCST9_MASK                                           (0x3F<<8)
#define     NAND_ECCST2_ECCST8_e                                              5
#define     NAND_ECCST2_ECCST8_SHIFT                                          0
#define     NAND_ECCST2_ECCST8_MASK                                           (0x3F<<0)

#define     NAND_ECCST3_ECCSTf_e                                              29
#define     NAND_ECCST3_ECCSTf_SHIFT                                          24
#define     NAND_ECCST3_ECCSTf_MASK                                           (0x3F<<24)
#define     NAND_ECCST3_ECCSTe_e                                              21
#define     NAND_ECCST3_ECCSTe_SHIFT                                          16
#define     NAND_ECCST3_ECCSTe_MASK                                           (0x3F<<16)
#define     NAND_ECCST3_ECCSTd_e                                              13
#define     NAND_ECCST3_ECCSTd_SHIFT                                          8
#define     NAND_ECCST3_ECCSTd_MASK                                           (0x3F<<8)
#define     NAND_ECCST3_ECCSTc_e                                              5
#define     NAND_ECCST3_ECCSTc_SHIFT                                          0
#define     NAND_ECCST3_ECCSTc_MASK                                           (0x3F<<0)

#define     NAND_UDATA0_DATA3_e                                               31
#define     NAND_UDATA0_DATA3_SHIFT                                           24
#define     NAND_UDATA0_DATA3_MASK                                            (0xFF<<24)
#define     NAND_UDATA0_DATA2_e                                               23
#define     NAND_UDATA0_DATA2_SHIFT                                           16
#define     NAND_UDATA0_DATA2_MASK                                            (0xFF<<16)
#define     NAND_UDATA0_DATA1_e                                               15
#define     NAND_UDATA0_DATA1_SHIFT                                           8
#define     NAND_UDATA0_DATA1_MASK                                            (0xFF<<8)
#define     NAND_UDATA0_DATA0_e                                               7
#define     NAND_UDATA0_DATA0_SHIFT                                           0
#define     NAND_UDATA0_DATA0_MASK                                            (0xFF<<0)

#define     NAND_UDATA1_DATA7_e                                               31
#define     NAND_UDATA1_DATA7_SHIFT                                           24
#define     NAND_UDATA1_DATA7_MASK                                            (0xFF<<24)
#define     NAND_UDATA1_DATA6_e                                               23
#define     NAND_UDATA1_DATA6_SHIFT                                           16
#define     NAND_UDATA1_DATA6_MASK                                            (0xFF<<16)
#define     NAND_UDATA1_DATA5_e                                               15
#define     NAND_UDATA1_DATA5_SHIFT                                           8
#define     NAND_UDATA1_DATA5_MASK                                            (0xFF<<8)
#define     NAND_UDATA1_DATA4_e                                               7
#define     NAND_UDATA1_DATA4_SHIFT                                           0
#define     NAND_UDATA1_DATA4_MASK                                            (0xFF<<0)

#define     NAND_UDATA2_DATAb_e                                               31
#define     NAND_UDATA2_DATAb_SHIFT                                           24
#define     NAND_UDATA2_DATAb_MASK                                            (0xFF<<24)
#define     NAND_UDATA2_DATAa_e                                               23
#define     NAND_UDATA2_DATAa_SHIFT                                           16
#define     NAND_UDATA2_DATAa_MASK                                            (0xFF<<16)
#define     NAND_UDATA2_DATA9_e                                               15
#define     NAND_UDATA2_DATA9_SHIFT                                           8
#define     NAND_UDATA2_DATA9_MASK                                            (0xFF<<8)
#define     NAND_UDATA2_DATA8_e                                               7
#define     NAND_UDATA2_DATA8_SHIFT                                           0
#define     NAND_UDATA2_DATA8_MASK                                            (0xFF<<0)

#define     NAND_UDATA3_DATAf_e                                               31
#define     NAND_UDATA3_DATAf_SHIFT                                           24
#define     NAND_UDATA3_DATAf_MASK                                            (0xFF<<24)
#define     NAND_UDATA3_DATAe_e                                               23
#define     NAND_UDATA3_DATAe_SHIFT                                           16
#define     NAND_UDATA3_DATAe_MASK                                            (0xFF<<16)
#define     NAND_UDATA3_DATAd_e                                               15
#define     NAND_UDATA3_DATAd_SHIFT                                           8
#define     NAND_UDATA3_DATAd_MASK                                            (0xFF<<8)
#define     NAND_UDATA3_DATAc_e                                               7
#define     NAND_UDATA3_DATAc_SHIFT                                           0
#define     NAND_UDATA3_DATAc_MASK                                            (0xFF<<0)

#define     NAND_TIMING_ODLY_e                                                31
#define     NAND_TIMING_ODLY_SHIFT                                            27
#define     NAND_TIMING_ODLY_MASK                                             (0x1F<<27)
#define     NAND_TIMING_IDLY_e                                                26
#define     NAND_TIMING_IDLY_SHIFT                                            22
#define     NAND_TIMING_IDLY_MASK                                             (0x1F<<22)
#define     NAND_TIMING_DQSLO_e                                               21
#define     NAND_TIMING_DQSLO_SHIFT                                           20
#define     NAND_TIMING_DQSLO_MASK                                            (0x3<<20)
#define     NAND_TIMING_DQSLI_e                                               19
#define     NAND_TIMING_DQSLI_SHIFT                                           18
#define     NAND_TIMING_DQSLI_MASK                                            (0x3<<18)
#define     NAND_TIMING_DCBAC_e                                               17
#define     NAND_TIMING_DCBAC_SHIFT                                           16
#define     NAND_TIMING_DCBAC_MASK                                            (0x3<<16)
#define     NAND_TIMING_TRPRE_e                                               15
#define     NAND_TIMING_TRPRE_SHIFT                                           14
#define     NAND_TIMING_TRPRE_MASK                                            (0x3<<14)
#define     NAND_TIMING_TWH_e                                                 13
#define     NAND_TIMING_TWH_SHIFT                                             12
#define     NAND_TIMING_TWH_MASK                                              (0x3<<12)
#define     NAND_TIMING_DCAD_e                                                11
#define     NAND_TIMING_DCAD_SHIFT                                            9
#define     NAND_TIMING_DCAD_MASK                                             (0x7<<9)
#define     NAND_TIMING_DCBD_e                                                8
#define     NAND_TIMING_DCBD_SHIFT                                            6
#define     NAND_TIMING_DCBD_MASK                                             (0x7<<6)
#define     NAND_TIMING_DCADD_e                                               5
#define     NAND_TIMING_DCADD_SHIFT                                           3
#define     NAND_TIMING_DCADD_MASK                                            (0x7<<3)
#define     NAND_TIMING_DCCA_e                                                2
#define     NAND_TIMING_DCCA_SHIFT                                            0
#define     NAND_TIMING_DCCA_MASK                                             (0x7<<0)

//--------------SDC-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SDC_BASE                                                          0xc0060000
#define     SD_EN                                                             (SDC_BASE+0x00)
#define     SD_CTL                                                            (SDC_BASE+0x04)
#define     SD_STATE                                                          (SDC_BASE+0x08)
#define     SD_CMD                                                            (SDC_BASE+0x0C)
#define     SD_ARG                                                            (SDC_BASE+0x10)
#define     SD_RSPBUF0                                                        (SDC_BASE+0x14)
#define     SD_RSPBUF1                                                        (SDC_BASE+0x18)
#define     SD_RSPBUF2                                                        (SDC_BASE+0x1C)
#define     SD_RSPBUF3                                                        (SDC_BASE+0x20)
#define     SD_RSPBUF4                                                        (SDC_BASE+0x24)
#define     SD_DAT                                                            (SDC_BASE+0x28)
#define     SD_BLK_SIZE                                                       (SDC_BASE+0x2C)
#define     SD_BLK_NUM                                                        (SDC_BASE+0x30)

//--------------Bits Location------------------------------------------//
#define     SD_EN_S18EN                                                       12
#define     SD_EN_CLKS                                                        8
#define     SD_EN_EN                                                          7
#define     SD_EN_AHB_DMA_SEL                                                 6
#define     SD_EN_SDIOEN                                                      3
#define     SD_EN_DATAWID_e                                                   1
#define     SD_EN_DATAWID_SHIFT                                               0
#define     SD_EN_DATAWID_MASK                                                (0x3<<0)

#define     SD_CTL_TOUTEN                                                     31
#define     SD_CTL_TOUTCNT                                                    30-24
#define     SD_CTL_RDELAY                                                     23-20
#define     SD_CTL_WDELAY                                                     19-16
#define     SD_CTL_CMDLEN                                                     13
#define     SD_CTL_SCC                                                        12
#define     SD_CTL_TCN_e                                                      11
#define     SD_CTL_TCN_SHIFT                                                  8
#define     SD_CTL_TCN_MASK                                                   (0xF<<8)
#define     SD_CTL_TS                                                         7
#define     SD_CTL_LBE                                                        6
#define     SD_CTL_C7EN                                                       5
#define     SD_CTL_TM_e                                                       3
#define     SD_CTL_TM_SHIFT                                                   0
#define     SD_CTL_TM_MASK                                                    (0xF<<0)

#define     SD_STATE_TOUTE                                                    15
#define     SD_STATE_BAEP                                                     14
#define     SD_STATE_MEMRDY                                                   12
#define     SD_STATE_CMDS                                                     11
#define     SD_STATE_DAT1S                                                    10
#define     SD_STATE_SDIOP                                                    9
#define     SD_STATE_SDIOEN                                                   8
#define     SD_STATE_DAT0S                                                    7
#define     SD_STATE_TEIE                                                     6
#define     SD_STATE_TEI                                                      5
#define     SD_STATE_CLNR                                                     4
#define     SD_STATE_CLC                                                      3
#define     SD_STATE_WC16ER                                                   2
#define     SD_STATE_RC16ER                                                   1
#define     SD_STATE_CRC7ER                                                   0

#define     SD_CMD_CMD_e                                                      7
#define     SD_CMD_CMD_SHIFT                                                  0
#define     SD_CMD_CMD_MASK                                                   (0xFF<<0)

#define     SD_ARG_ARG_e                                                      31
#define     SD_ARG_ARG_SHIFT                                                  0
#define     SD_ARG_ARG_MASK                                                   (0xFFFFFFFF<<0)

#define     SD_RSPBUF0_RSP0_e                                                 31
#define     SD_RSPBUF0_RSP0_SHIFT                                             0
#define     SD_RSPBUF0_RSP0_MASK                                              (0xFFFFFFFF<<0)

#define     SD_RSPBUF1_RSP1_e                                                 31
#define     SD_RSPBUF1_RSP1_SHIFT                                             0
#define     SD_RSPBUF1_RSP1_MASK                                              (0xFFFFFFFF<<0)

#define     SD_RSPBUF2_RSP2_e                                                 31
#define     SD_RSPBUF2_RSP2_SHIFT                                             0
#define     SD_RSPBUF2_RSP2_MASK                                              (0xFFFFFFFF<<0)

#define     SD_RSPBUF3_RSP3_e                                                 31
#define     SD_RSPBUF3_RSP3_SHIFT                                             0
#define     SD_RSPBUF3_RSP3_MASK                                              (0xFFFFFFFF<<0)

#define     SD_RSPBUF4_RSP4_e                                                 7
#define     SD_RSPBUF4_RSP4_SHIFT                                             0
#define     SD_RSPBUF4_RSP4_MASK                                              (0xFF<<0)

#define     SD_DAT_DATA_e                                                     31
#define     SD_DAT_DATA_SHIFT                                                 0
#define     SD_DAT_DATA_MASK                                                  (0xFFFFFFFF<<0)

#define     SD_BLK_SIZE_BC_e                                                  9
#define     SD_BLK_SIZE_BC_SHIFT                                              0
#define     SD_BLK_SIZE_BC_MASK                                               (0x3FF<<0)

#define     SD_BLK_NUM_BN_e                                                   15
#define     SD_BLK_NUM_BN_SHIFT                                               0
#define     SD_BLK_NUM_BN_MASK                                                (0xFFFF<<0)

//--------------USB_Controller_Registers-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     USB_Controller_Registers_BASE                                     0xC0080000
#define     USBIRQ                                                            (USB_Controller_Registers_BASE+0x1BC)
#define     USBstate                                                          (USB_Controller_Registers_BASE+0x1BD)
#define     USBCtrl                                                           (USB_Controller_Registers_BASE+0x1BE)
#define     USBstatus                                                         (USB_Controller_Registers_BASE+0x1BF)
#define     USBien                                                            (USB_Controller_Registers_BASE+0x1C0)
#define     Taaidlbdis                                                        (USB_Controller_Registers_BASE+0x1C1)
#define     Tawaitbcon                                                        (USB_Controller_Registers_BASE+0x1C2)
#define     Tbvbuspls                                                         (USB_Controller_Registers_BASE+0x1C3)
#define     Tbvbusdispls                                                      (USB_Controller_Registers_BASE+0x1C7)
#define     Hcep0ctrl                                                         (USB_Controller_Registers_BASE+0x0C0)
#define     Hcout1ctrl                                                        (USB_Controller_Registers_BASE+0x0C4)
#define     Hcout2ctrl                                                        (USB_Controller_Registers_BASE+0x0C8)
#define     Hcout0err                                                         (USB_Controller_Registers_BASE+0x0C1)
#define     Hcout1err                                                         (USB_Controller_Registers_BASE+0x0C5)
#define     Hcout2err                                                         (USB_Controller_Registers_BASE+0x0C9)
#define     Hcin1ctrl                                                         (USB_Controller_Registers_BASE+0x0C6)
#define     Hcin2ctrl                                                         (USB_Controller_Registers_BASE+0x0CA)
#define     Hcin0err                                                          (USB_Controller_Registers_BASE+0x0C3)
#define     Hcin1err                                                          (USB_Controller_Registers_BASE+0x0C7)
#define     Hcin2err                                                          (USB_Controller_Registers_BASE+0x0CB)
#define     Hcportctrl                                                        (USB_Controller_Registers_BASE+0x1AB)
#define     Hcfrmnl                                                           (USB_Controller_Registers_BASE+0x1AC)
#define     Hcfrmnh                                                           (USB_Controller_Registers_BASE+0x1AD)
#define     Hcfrmremainl                                                      (USB_Controller_Registers_BASE+0x1AE)
#define     Hcfrmremainh                                                      (USB_Controller_Registers_BASE+0x1AF)
#define     HCIN02ERRIRQ                                                      (USB_Controller_Registers_BASE+0x1B4)
#define     HCOUT02ERRIRQ                                                     (USB_Controller_Registers_BASE+0x1B6)
#define     HCIN02ERRIEN                                                      (USB_Controller_Registers_BASE+0x1B8)
#define     HCOUT02ERRIEN                                                     (USB_Controller_Registers_BASE+0x1BA)
#define     Hcin0maxpck                                                       (USB_Controller_Registers_BASE+0x1E0)
#define     Hcin1maxpckl                                                      (USB_Controller_Registers_BASE+0x1E2)
#define     Hcin2maxpckl                                                      (USB_Controller_Registers_BASE+0x1E4)
#define     Hcin1maxpckh                                                      (USB_Controller_Registers_BASE+0x1E3)
#define     Hcin2maxpckh                                                      (USB_Controller_Registers_BASE+0x1E5)
#define     Hcout1maxpckl                                                     (USB_Controller_Registers_BASE+0x3E2)
#define     Hcout2maxpckl                                                     (USB_Controller_Registers_BASE+0x3E4)
#define     Hcout1maxpckh                                                     (USB_Controller_Registers_BASE+0x3E3)
#define     Hcout2maxpckh                                                     (USB_Controller_Registers_BASE+0x3E5)
#define     Out0bc_hcin0bc                                                    (USB_Controller_Registers_BASE+0x000)
#define     In0bc_hcout0bc                                                    (USB_Controller_Registers_BASE+0x001)
#define     Ep0cs_hcep0cs                                                     (USB_Controller_Registers_BASE+0x002)
#define     Out1bcl_hcin1bcl                                                  (USB_Controller_Registers_BASE+0x008)
#define     Out2bcl_hcin2bcl                                                  (USB_Controller_Registers_BASE+0x010)
#define     Out1bch_hcin1bch                                                  (USB_Controller_Registers_BASE+0x009)
#define     Out2bch_hcin2bch                                                  (USB_Controller_Registers_BASE+0x011)
#define     Out1cs_hcin1cs                                                    (USB_Controller_Registers_BASE+0x00B)
#define     Out2cs_hcin2cs                                                    (USB_Controller_Registers_BASE+0x013)
#define     Out1ctrl_hcin1ctrl                                                (USB_Controller_Registers_BASE+0x00A)
#define     Out2ctrl_hcin2ctrl                                                (USB_Controller_Registers_BASE+0x012)
#define     In1bcl_hcout1bcl                                                  (USB_Controller_Registers_BASE+0x00C)
#define     In2bcl_hcout2bcl                                                  (USB_Controller_Registers_BASE+0x014)
#define     In1bch_hcout1bch                                                  (USB_Controller_Registers_BASE+0x00D)
#define     In2bch_hcout2bch                                                  (USB_Controller_Registers_BASE+0x015)
#define     In1cs_hcout1cs                                                    (USB_Controller_Registers_BASE+0x00F)
#define     In2cs_hcout2cs                                                    (USB_Controller_Registers_BASE+0x017)
#define     In1ctrl_hcout1ctrl                                                (USB_Controller_Registers_BASE+0x00E)
#define     In2ctrl_hcout2ctrl                                                (USB_Controller_Registers_BASE+0x016)
#define     Fifo1dat                                                          (USB_Controller_Registers_BASE+0x084)
#define     Fifo2dat                                                          (USB_Controller_Registers_BASE+0x088)
#define     Ep0indata                                                         (USB_Controller_Registers_BASE+0x100)
#define     Ep0outdata                                                        (USB_Controller_Registers_BASE+0x140)
#define     Setupdat0                                                         (USB_Controller_Registers_BASE+0x180)
#define     Setupdat1                                                         (USB_Controller_Registers_BASE+0x181)
#define     Setupdat2                                                         (USB_Controller_Registers_BASE+0x182)
#define     Setupdat3                                                         (USB_Controller_Registers_BASE+0x183)
#define     Setupdat4                                                         (USB_Controller_Registers_BASE+0x184)
#define     Setupdat5                                                         (USB_Controller_Registers_BASE+0x185)
#define     Setupdat6                                                         (USB_Controller_Registers_BASE+0x186)
#define     Setupdat7                                                         (USB_Controller_Registers_BASE+0x187)
#define     OUT02IRQ                                                          (USB_Controller_Registers_BASE+0x1A8)
#define     IN02IRQ                                                           (USB_Controller_Registers_BASE+0x188)
#define     Usbirq_hcusbirq                                                   (USB_Controller_Registers_BASE+0x18C)
#define     OUT02PNGIRQ                                                       (USB_Controller_Registers_BASE+0x18E)
#define     OUT02IEN                                                          (USB_Controller_Registers_BASE+0x196)
#define     IN02IEN                                                           (USB_Controller_Registers_BASE+0x194)
#define     Usbien_hcusbien                                                   (USB_Controller_Registers_BASE+0x198)
#define     OUT02PNGIEN                                                       (USB_Controller_Registers_BASE+0x19A)
#define     IN02TOKIRQ                                                        (USB_Controller_Registers_BASE+0x190)
#define     OUT02TOKIRQ                                                       (USB_Controller_Registers_BASE+0x191)
#define     IN02TOKIEN                                                        (USB_Controller_Registers_BASE+0x19C)
#define     OUT02TOKIEN                                                       (USB_Controller_Registers_BASE+0x19D)
#define     IVECT                                                             (USB_Controller_Registers_BASE+0x1A0)
#define     EPRST                                                             (USB_Controller_Registers_BASE+0x1A2)
#define     UsbCTRL_STUS                                                      (USB_Controller_Registers_BASE+0x1A3)
#define     FrmCNTL                                                           (USB_Controller_Registers_BASE+0x1A4)
#define     FrmCNTH                                                           (USB_Controller_Registers_BASE+0x1A5)
#define     Fnaddr                                                            (USB_Controller_Registers_BASE+0x1A6)
#define     Clkgate                                                           (USB_Controller_Registers_BASE+0x1A7)
#define     Fifoctrl                                                          (USB_Controller_Registers_BASE+0x1A8)
#define     OUT1STADDR                                                        (USB_Controller_Registers_BASE+0x304)
#define     OUT2STADDR                                                        (USB_Controller_Registers_BASE+0x308)
#define     IN1STADDR                                                         (USB_Controller_Registers_BASE+0x344)
#define     IN2STADDR                                                         (USB_Controller_Registers_BASE+0x348)
#define     USBEIRQ                                                           (USB_Controller_Registers_BASE+0x400)
#define     NAKOUTCTRL                                                        (USB_Controller_Registers_BASE+0x401)
#define     HCINCTRL                                                          (USB_Controller_Registers_BASE+0x402)
#define     OSHRTPCKIR                                                        (USB_Controller_Registers_BASE+0x403)
#define     USBDEBUG                                                          (USB_Controller_Registers_BASE+0x404)
#define     HCOUTEMPCON                                                       (USB_Controller_Registers_BASE+0x406)
#define     VDCTRL                                                            (USB_Controller_Registers_BASE+0x408)
#define     VDSTAT                                                            (USB_Controller_Registers_BASE+0x409)
#define     MEMORYCTRL                                                        (USB_Controller_Registers_BASE+0X40A)
#define     BKDOOR                                                            (USB_Controller_Registers_BASE+0x40B)
#define     DMAEPSEL                                                          (USB_Controller_Registers_BASE+0x40C)
#define     OTGSTAIEN                                                         (USB_Controller_Registers_BASE+0x410)
#define     OTGSTAIR                                                          (USB_Controller_Registers_BASE+0x411)
#define     HCIN1_COUNTL                                                      (USB_Controller_Registers_BASE+0x414)
#define     HCIN1_COUNTH                                                      (USB_Controller_Registers_BASE+0x415)
#define     HCIN2_COUNTL                                                      (USB_Controller_Registers_BASE+0x416)
#define     HCIN2_COUNTH                                                      (USB_Controller_Registers_BASE+0x417)
#define     IDVBUSCTRL                                                        (USB_Controller_Registers_BASE+0x418)
#define     LINESTATUS                                                        (USB_Controller_Registers_BASE+0x419)
#define     DPDMCTRL                                                          (USB_Controller_Registers_BASE+0x41A)
#define     USB_PHYCTRL                                                       (USB_Controller_Registers_BASE+0x41B)
#define     DMANORMALLENL                                                     (USB_Controller_Registers_BASE+0x41C)
#define     DMANORMALLENH                                                     (USB_Controller_Registers_BASE+0x41D)

//--------------Bits Location------------------------------------------//
#define     USBIRQ_periphirq                                                  4
#define     USBIRQ_vbuserrirq                                                 3
#define     USBIRQ_locsofirq                                                  2
#define     USBIRQ_srpdetirq                                                  1
#define     USBIRQ_idleirq                                                    0

#define     USBstate_st3_st0_e                                                3
#define     USBstate_st3_st0_SHIFT                                            0
#define     USBstate_st3_st0_MASK                                             (0xF<<0)

#define     USBCtrl_forcebconn                                                7
#define     USBCtrl_srpdatdeten                                               5
#define     USBCtrl_srpvbusdeten                                              4
#define     USBCtrl_bhnpen                                                    3
#define     USBCtrl_asetbhnpen                                                2
#define     USBCtrl_abusdrop                                                  1
#define     USBCtrl_busreq                                                    0

#define     USBstatus_id                                                      6
#define     USBstatus_avbusval                                                5
#define     USBstatus_bsessend                                                4
#define     USBstatus_asessval                                                3
#define     USBstatus_bsessval                                                2
#define     USBstatus_conn                                                    1
#define     USBstatus_bse0srp                                                 0

#define     USBien_periphien                                                  4
#define     USBien_vbuserrien                                                 3
#define     USBien_locsofien                                                  2
#define     USBien_srpdetien                                                  1
#define     USBien_idleien                                                    0

#define     Taaidlbdis_taaidlbdis_e                                           7
#define     Taaidlbdis_taaidlbdis_SHIFT                                       0
#define     Taaidlbdis_taaidlbdis_MASK                                        (0xFF<<0)

#define     Tawaitbcon_tawaitbcon_e                                           7
#define     Tawaitbcon_tawaitbcon_SHIFT                                       0
#define     Tawaitbcon_tawaitbcon_MASK                                        (0xFF<<0)

#define     Tbvbuspls_tbvbuspls_e                                             7
#define     Tbvbuspls_tbvbuspls_SHIFT                                         0
#define     Tbvbuspls_tbvbuspls_MASK                                          (0xFF<<0)

#define     Tbvbusdispls_tbvbusdispls_e                                       7
#define     Tbvbusdispls_tbvbusdispls_SHIFT                                   0
#define     Tbvbusdispls_tbvbusdispls_MASK                                    (0xFF<<0)

#define     Hcep0ctrl_HcEp0Ctrl_e                                             3
#define     Hcep0ctrl_HcEp0Ctrl_SHIFT                                         0
#define     Hcep0ctrl_HcEp0Ctrl_MASK                                          (0xF<<0)

#define     Hcout1ctrl_HcEp1Ctrl_e                                            3
#define     Hcout1ctrl_HcEp1Ctrl_SHIFT                                        0
#define     Hcout1ctrl_HcEp1Ctrl_MASK                                         (0xF<<0)

#define     Hcout2ctrl_HcEp2Ctrl_e                                            3
#define     Hcout2ctrl_HcEp2Ctrl_SHIFT                                        0
#define     Hcout2ctrl_HcEp2Ctrl_MASK                                         (0xF<<0)

#define     Hcout0err_doping                                                  6
#define     Hcout0err_Resend                                                  5
#define     Hcout0err_Errtype_e                                               4
#define     Hcout0err_Errtype_SHIFT                                           2
#define     Hcout0err_Errtype_MASK                                            (0x7<<2)
#define     Hcout0err_Errcount_e                                              1
#define     Hcout0err_Errcount_SHIFT                                          0
#define     Hcout0err_Errcount_MASK                                           (0x3<<0)

#define     Hcout1err_doping                                                  6
#define     Hcout1err_Resend                                                  5
#define     Hcout1err_Errtype_e                                               4
#define     Hcout1err_Errtype_SHIFT                                           2
#define     Hcout1err_Errtype_MASK                                            (0x7<<2)
#define     Hcout1err_Errcount_e                                              1
#define     Hcout1err_Errcount_SHIFT                                          0
#define     Hcout1err_Errcount_MASK                                           (0x3<<0)

#define     Hcout2err_doping                                                  6
#define     Hcout2err_Resend                                                  5
#define     Hcout2err_Errtype_e                                               4
#define     Hcout2err_Errtype_SHIFT                                           2
#define     Hcout2err_Errtype_MASK                                            (0x7<<2)
#define     Hcout2err_Errcount_e                                              1
#define     Hcout2err_Errcount_SHIFT                                          0
#define     Hcout2err_Errcount_MASK                                           (0x3<<0)

#define     Hcin1ctrl_hcinxctrl_e                                             3
#define     Hcin1ctrl_hcinxctrl_SHIFT                                         0
#define     Hcin1ctrl_hcinxctrl_MASK                                          (0xF<<0)

#define     Hcin2ctrl_hcinxctrl_e                                             3
#define     Hcin2ctrl_hcinxctrl_SHIFT                                         0
#define     Hcin2ctrl_hcinxctrl_MASK                                          (0xF<<0)

#define     Hcin0err_underrien                                                7
#define     Hcin0err_Resend                                                   5
#define     Hcin0err_Errtype_e                                                4
#define     Hcin0err_Errtype_SHIFT                                            2
#define     Hcin0err_Errtype_MASK                                             (0x7<<2)
#define     Hcin0err_Errcount_e                                               1
#define     Hcin0err_Errcount_SHIFT                                           0
#define     Hcin0err_Errcount_MASK                                            (0x3<<0)

#define     Hcin1err_underrien                                                7
#define     Hcin1err_Resend                                                   5
#define     Hcin1err_Errtype_e                                                4
#define     Hcin1err_Errtype_SHIFT                                            2
#define     Hcin1err_Errtype_MASK                                             (0x7<<2)
#define     Hcin1err_Errcount_e                                               1
#define     Hcin1err_Errcount_SHIFT                                           0
#define     Hcin1err_Errcount_MASK                                            (0x3<<0)

#define     Hcin2err_underrien                                                7
#define     Hcin2err_Resend                                                   5
#define     Hcin2err_Errtype_e                                                4
#define     Hcin2err_Errtype_SHIFT                                            2
#define     Hcin2err_Errtype_MASK                                             (0x7<<2)
#define     Hcin2err_Errcount_e                                               1
#define     Hcin2err_Errcount_SHIFT                                           0
#define     Hcin2err_Errcount_MASK                                            (0x3<<0)

#define     Hcportctrl_RstLengthCtrl_e                                        7
#define     Hcportctrl_RstLengthCtrl_SHIFT                                    6
#define     Hcportctrl_RstLengthCtrl_MASK                                     (0x3<<6)
#define     Hcportctrl_PortRst                                                5
#define     Hcportctrl_Testm_e                                                4
#define     Hcportctrl_Testm_SHIFT                                            0
#define     Hcportctrl_Testm_MASK                                             (0x1F<<0)

#define     Hcfrmnl_HcfrmL_e                                                  7
#define     Hcfrmnl_HcfrmL_SHIFT                                              0
#define     Hcfrmnl_HcfrmL_MASK                                               (0xFF<<0)

#define     Hcfrmnh_HcfrmH_e                                                  7
#define     Hcfrmnh_HcfrmH_SHIFT                                              0
#define     Hcfrmnh_HcfrmH_MASK                                               (0xFF<<0)

#define     Hcfrmremainl_hcfrmrmL_e                                           7
#define     Hcfrmremainl_hcfrmrmL_SHIFT                                       0
#define     Hcfrmremainl_hcfrmrmL_MASK                                        (0xFF<<0)

#define     Hcfrmremainh_hcfrmrmH_e                                           3
#define     Hcfrmremainh_hcfrmrmH_SHIFT                                       0
#define     Hcfrmremainh_hcfrmrmH_MASK                                        (0xF<<0)

#define     HCIN02ERRIRQ_hcinXerrirq_e                                        4
#define     HCIN02ERRIRQ_hcinXerrirq_SHIFT                                    0
#define     HCIN02ERRIRQ_hcinXerrirq_MASK                                     (0x1F<<0)

#define     HCOUT02ERRIRQ_hcoutxerrirq_e                                      4
#define     HCOUT02ERRIRQ_hcoutxerrirq_SHIFT                                  0
#define     HCOUT02ERRIRQ_hcoutxerrirq_MASK                                   (0x1F<<0)

#define     HCIN02ERRIEN_inxerrien_e                                          2
#define     HCIN02ERRIEN_inxerrien_SHIFT                                      0
#define     HCIN02ERRIEN_inxerrien_MASK                                       (0x7<<0)

#define     HCOUT02ERRIEN_outxerrien_e                                        2
#define     HCOUT02ERRIEN_outxerrien_SHIFT                                    0
#define     HCOUT02ERRIEN_outxerrien_MASK                                     (0x7<<0)

#define     Hcin0maxpck_HcIn0MaxP_e                                           6
#define     Hcin0maxpck_HcIn0MaxP_SHIFT                                       0
#define     Hcin0maxpck_HcIn0MaxP_MASK                                        (0x7F<<0)

#define     Hcin1maxpckl_HcIn1MaxPL_e                                         7
#define     Hcin1maxpckl_HcIn1MaxPL_SHIFT                                     0
#define     Hcin1maxpckl_HcIn1MaxPL_MASK                                      (0xFF<<0)

#define     Hcin2maxpckl_HcIn2MaxPL_e                                         7
#define     Hcin2maxpckl_HcIn2MaxPL_SHIFT                                     0
#define     Hcin2maxpckl_HcIn2MaxPL_MASK                                      (0xFF<<0)

#define     Hcin1maxpckh_HcIn1MaxPH_e                                         2
#define     Hcin1maxpckh_HcIn1MaxPH_SHIFT                                     0
#define     Hcin1maxpckh_HcIn1MaxPH_MASK                                      (0x7<<0)

#define     Hcin2maxpckh_HcIn2MaxPH_e                                         2
#define     Hcin2maxpckh_HcIn2MaxPH_SHIFT                                     0
#define     Hcin2maxpckh_HcIn2MaxPH_MASK                                      (0x7<<0)

#define     Hcout1maxpckl_HcOut1MaxPL_e                                       7
#define     Hcout1maxpckl_HcOut1MaxPL_SHIFT                                   0
#define     Hcout1maxpckl_HcOut1MaxPL_MASK                                    (0xFF<<0)

#define     Hcout2maxpckl_HcOut2MaxPL_e                                       7
#define     Hcout2maxpckl_HcOut2MaxPL_SHIFT                                   0
#define     Hcout2maxpckl_HcOut2MaxPL_MASK                                    (0xFF<<0)

#define     Hcout1maxpckh_HcOut1MaxPH_e                                       2
#define     Hcout1maxpckh_HcOut1MaxPH_SHIFT                                   0
#define     Hcout1maxpckh_HcOut1MaxPH_MASK                                    (0x7<<0)

#define     Hcout2maxpckh_HcOut2MaxPH_e                                       2
#define     Hcout2maxpckh_HcOut2MaxPH_SHIFT                                   0
#define     Hcout2maxpckh_HcOut2MaxPH_MASK                                    (0x7<<0)

#define     Out0bc_hcin0bc_Ep0Outbc_e                                         6
#define     Out0bc_hcin0bc_Ep0Outbc_SHIFT                                     0
#define     Out0bc_hcin0bc_Ep0Outbc_MASK                                      (0x7F<<0)

#define     In0bc_hcout0bc_Ep0Inbc_e                                          6
#define     In0bc_hcout0bc_Ep0Inbc_SHIFT                                      0
#define     In0bc_hcout0bc_Ep0Inbc_MASK                                       (0x7F<<0)

#define     Ep0cs_hcep0cs_hcsettoggle                                         6
#define     Ep0cs_hcep0cs_hcclrtoggle                                         5
#define     Ep0cs_hcep0cs_hcset                                               4
#define     Ep0cs_hcep0cs_Outbsy_hcinbsy                                      3
#define     Ep0cs_hcep0cs_Inbsy_hcoutbsy                                      2
#define     Ep0cs_hcep0cs_hsnak                                               1
#define     Ep0cs_hcep0cs_stall                                               0

#define     Out1bcl_hcin1bcl_Ep1bcL_e                                         7
#define     Out1bcl_hcin1bcl_Ep1bcL_SHIFT                                     0
#define     Out1bcl_hcin1bcl_Ep1bcL_MASK                                      (0xFF<<0)

#define     Out2bcl_hcin2bcl_Ep2bcL_e                                         7
#define     Out2bcl_hcin2bcl_Ep2bcL_SHIFT                                     0
#define     Out2bcl_hcin2bcl_Ep2bcL_MASK                                      (0xFF<<0)

#define     Out1bch_hcin1bch_Ep1bcH_e                                         3
#define     Out1bch_hcin1bch_Ep1bcH_SHIFT                                     0
#define     Out1bch_hcin1bch_Ep1bcH_MASK                                      (0xF<<0)

#define     Out2bch_hcin2bch_Ep2bcH_e                                         3
#define     Out2bch_hcin2bch_Ep2bcH_SHIFT                                     0
#define     Out2bch_hcin2bch_Ep2bcH_MASK                                      (0xF<<0)

#define     Out1cs_hcin1cs_Autoin_hcautoout                                   4
#define     Out1cs_hcin1cs_Npak_hcnpak_e                                      3
#define     Out1cs_hcin1cs_Npak_hcnpak_SHIFT                                  2
#define     Out1cs_hcin1cs_Npak_hcnpak_MASK                                   (0x3<<2)
#define     Out1cs_hcin1cs_Busy_hcbusy                                        1
#define     Out1cs_hcin1cs_err                                                0

#define     Out2cs_hcin2cs_Autoin_hcautoout                                   4
#define     Out2cs_hcin2cs_Npak_hcnpak_e                                      3
#define     Out2cs_hcin2cs_Npak_hcnpak_SHIFT                                  2
#define     Out2cs_hcin2cs_Npak_hcnpak_MASK                                   (0x3<<2)
#define     Out2cs_hcin2cs_Busy_hcbusy                                        1
#define     Out2cs_hcin2cs_err                                                0

#define     Out1ctrl_hcin1ctrl_val                                            7
#define     Out1ctrl_hcin1ctrl_stall                                          6
#define     Out1ctrl_hcin1ctrl_Hcinisod_e                                     5
#define     Out1ctrl_hcin1ctrl_Hcinisod_SHIFT                                 4
#define     Out1ctrl_hcin1ctrl_Hcinisod_MASK                                  (0x3<<4)
#define     Out1ctrl_hcin1ctrl_Type_Hctype_e                                  3
#define     Out1ctrl_hcin1ctrl_Type_Hctype_SHIFT                              2
#define     Out1ctrl_hcin1ctrl_Type_Hctype_MASK                               (0x3<<2)
#define     Out1ctrl_hcin1ctrl_Buf_hcbuf_e                                    1
#define     Out1ctrl_hcin1ctrl_Buf_hcbuf_SHIFT                                0
#define     Out1ctrl_hcin1ctrl_Buf_hcbuf_MASK                                 (0x3<<0)

#define     Out2ctrl_hcin2ctrl_val                                            7
#define     Out2ctrl_hcin2ctrl_stall                                          6
#define     Out2ctrl_hcin2ctrl_Hcinisod_e                                     5
#define     Out2ctrl_hcin2ctrl_Hcinisod_SHIFT                                 4
#define     Out2ctrl_hcin2ctrl_Hcinisod_MASK                                  (0x3<<4)
#define     Out2ctrl_hcin2ctrl_Type_Hctype_e                                  3
#define     Out2ctrl_hcin2ctrl_Type_Hctype_SHIFT                              2
#define     Out2ctrl_hcin2ctrl_Type_Hctype_MASK                               (0x3<<2)
#define     Out2ctrl_hcin2ctrl_Buf_hcbuf_e                                    1
#define     Out2ctrl_hcin2ctrl_Buf_hcbuf_SHIFT                                0
#define     Out2ctrl_hcin2ctrl_Buf_hcbuf_MASK                                 (0x3<<0)

#define     In1bcl_hcout1bcl_Ep1bcL_e                                         7
#define     In1bcl_hcout1bcl_Ep1bcL_SHIFT                                     0
#define     In1bcl_hcout1bcl_Ep1bcL_MASK                                      (0xFF<<0)

#define     In2bcl_hcout2bcl_Ep2bcL_e                                         7
#define     In2bcl_hcout2bcl_Ep2bcL_SHIFT                                     0
#define     In2bcl_hcout2bcl_Ep2bcL_MASK                                      (0xFF<<0)

#define     In1bch_hcout1bch_Ep1bcH_e                                         2
#define     In1bch_hcout1bch_Ep1bcH_SHIFT                                     0
#define     In1bch_hcout1bch_Ep1bcH_MASK                                      (0x7<<0)

#define     In2bch_hcout2bch_Ep2bcH_e                                         2
#define     In2bch_hcout2bch_Ep2bcH_SHIFT                                     0
#define     In2bch_hcout2bch_Ep2bcH_MASK                                      (0x7<<0)

#define     In1cs_hcout1cs_Autoin_hcautoout                                   4
#define     In1cs_hcout1cs_Npak_hcnpak_e                                      3
#define     In1cs_hcout1cs_Npak_hcnpak_SHIFT                                  2
#define     In1cs_hcout1cs_Npak_hcnpak_MASK                                   (0x3<<2)
#define     In1cs_hcout1cs_Busy_hcbusy                                        1
#define     In1cs_hcout1cs_err                                                0

#define     In2cs_hcout2cs_Autoin_hcautoout                                   4
#define     In2cs_hcout2cs_Npak_hcnpak_e                                      3
#define     In2cs_hcout2cs_Npak_hcnpak_SHIFT                                  2
#define     In2cs_hcout2cs_Npak_hcnpak_MASK                                   (0x3<<2)
#define     In2cs_hcout2cs_Busy_hcbusy                                        1
#define     In2cs_hcout2cs_err                                                0

#define     In1ctrl_hcout1ctrl_val                                            7
#define     In1ctrl_hcout1ctrl_stall                                          6
#define     In1ctrl_hcout1ctrl_Isod_hcisod_e                                  5
#define     In1ctrl_hcout1ctrl_Isod_hcisod_SHIFT                              4
#define     In1ctrl_hcout1ctrl_Isod_hcisod_MASK                               (0x3<<4)
#define     In1ctrl_hcout1ctrl_Type_Hctype_e                                  3
#define     In1ctrl_hcout1ctrl_Type_Hctype_SHIFT                              2
#define     In1ctrl_hcout1ctrl_Type_Hctype_MASK                               (0x3<<2)
#define     In1ctrl_hcout1ctrl_Buf_hcbuf_e                                    1
#define     In1ctrl_hcout1ctrl_Buf_hcbuf_SHIFT                                0
#define     In1ctrl_hcout1ctrl_Buf_hcbuf_MASK                                 (0x3<<0)

#define     In2ctrl_hcout2ctrl_val                                            7
#define     In2ctrl_hcout2ctrl_stall                                          6
#define     In2ctrl_hcout2ctrl_Isod_hcisod_e                                  5
#define     In2ctrl_hcout2ctrl_Isod_hcisod_SHIFT                              4
#define     In2ctrl_hcout2ctrl_Isod_hcisod_MASK                               (0x3<<4)
#define     In2ctrl_hcout2ctrl_Type_Hctype_e                                  3
#define     In2ctrl_hcout2ctrl_Type_Hctype_SHIFT                              2
#define     In2ctrl_hcout2ctrl_Type_Hctype_MASK                               (0x3<<2)
#define     In2ctrl_hcout2ctrl_Buf_hcbuf_e                                    1
#define     In2ctrl_hcout2ctrl_Buf_hcbuf_SHIFT                                0
#define     In2ctrl_hcout2ctrl_Buf_hcbuf_MASK                                 (0x3<<0)

#define     Fifo1dat_Fifo1dat_e                                               31
#define     Fifo1dat_Fifo1dat_SHIFT                                           0
#define     Fifo1dat_Fifo1dat_MASK                                            (0xFFFFFFFF<<0)

#define     Fifo2dat_Fifo2dat_e                                               31
#define     Fifo2dat_Fifo2dat_SHIFT                                           0
#define     Fifo2dat_Fifo2dat_MASK                                            (0xFFFFFFFF<<0)

#define     Ep0indata_Ep0indata_e                                             511
#define     Ep0indata_Ep0indata_SHIFT                                         0
#define     Ep0indata_Ep0indata_MASK                                          (0xFFFFFFFF<<0)

#define     Ep0outdata_Ep0outdata_e                                           511
#define     Ep0outdata_Ep0outdata_SHIFT                                       0
#define     Ep0outdata_Ep0outdata_MASK                                        (0xFFFFFFFF<<0)

#define     Setupdat0_Setupdat_e                                              7
#define     Setupdat0_Setupdat_SHIFT                                          0
#define     Setupdat0_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat1_Setupdat_e                                              7
#define     Setupdat1_Setupdat_SHIFT                                          0
#define     Setupdat1_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat2_Setupdat_e                                              7
#define     Setupdat2_Setupdat_SHIFT                                          0
#define     Setupdat2_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat3_Setupdat_e                                              7
#define     Setupdat3_Setupdat_SHIFT                                          0
#define     Setupdat3_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat4_Setupdat_e                                              7
#define     Setupdat4_Setupdat_SHIFT                                          0
#define     Setupdat4_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat5_Setupdat_e                                              7
#define     Setupdat5_Setupdat_SHIFT                                          0
#define     Setupdat5_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat6_Setupdat_e                                              7
#define     Setupdat6_Setupdat_SHIFT                                          0
#define     Setupdat6_Setupdat_MASK                                           (0xFF<<0)

#define     Setupdat7_Setupdat_e                                              7
#define     Setupdat7_Setupdat_SHIFT                                          0
#define     Setupdat7_Setupdat_MASK                                           (0xFF<<0)

#define     OUT02IRQ_Outxirq_hcinxirq_e                                       2
#define     OUT02IRQ_Outxirq_hcinxirq_SHIFT                                   0
#define     OUT02IRQ_Outxirq_hcinxirq_MASK                                    (0x7<<0)

#define     IN02IRQ_Inxirq_hcoutxirq_e                                        2
#define     IN02IRQ_Inxirq_hcoutxirq_SHIFT                                    0
#define     IN02IRQ_Inxirq_hcoutxirq_MASK                                     (0x7<<0)

#define     Usbirq_hcusbirq_HSirq_hcHSirq                                     5
#define     Usbirq_hcusbirq_RstIrq                                            4
#define     Usbirq_hcusbirq_SuspIrq                                           3
#define     Usbirq_hcusbirq_SutokIrq                                          2
#define     Usbirq_hcusbirq_sofir                                             1
#define     Usbirq_hcusbirq_sudavir                                           0

#define     OUT02PNGIRQ_outxpng_e                                             2
#define     OUT02PNGIRQ_outxpng_SHIFT                                         0
#define     OUT02PNGIRQ_outxpng_MASK                                          (0x7<<0)

#define     OUT02IEN_Outxien_hcinxien_e                                       2
#define     OUT02IEN_Outxien_hcinxien_SHIFT                                   0
#define     OUT02IEN_Outxien_hcinxien_MASK                                    (0x7<<0)

#define     IN02IEN_Inxien_hcoutxien_e                                        2
#define     IN02IEN_Inxien_hcoutxien_SHIFT                                    0
#define     IN02IEN_Inxien_hcoutxien_MASK                                     (0x7<<0)

#define     Usbien_hcusbien_HSien_hcHSien                                     5
#define     Usbien_hcusbien_RstIen                                            4
#define     Usbien_hcusbien_SuspIen                                           3
#define     Usbien_hcusbien_SutokIen                                          2
#define     Usbien_hcusbien_SOFIen                                            1
#define     Usbien_hcusbien_SudavIen                                          0

#define     OUT02PNGIEN_outxpie_e                                             2
#define     OUT02PNGIEN_outxpie_SHIFT                                         0
#define     OUT02PNGIEN_outxpie_MASK                                          (0x7<<0)

#define     IN02TOKIRQ_In02tokirq_e                                           2
#define     IN02TOKIRQ_In02tokirq_SHIFT                                       0
#define     IN02TOKIRQ_In02tokirq_MASK                                        (0x7<<0)

#define     OUT02TOKIRQ_Out02tokirq_e                                         2
#define     OUT02TOKIRQ_Out02tokirq_SHIFT                                     0
#define     OUT02TOKIRQ_Out02tokirq_MASK                                      (0x7<<0)

#define     IN02TOKIEN_In02tokien_e                                           2
#define     IN02TOKIEN_In02tokien_SHIFT                                       0
#define     IN02TOKIEN_In02tokien_MASK                                        (0x7<<0)

#define     OUT02TOKIEN_out02tokien_e                                         2
#define     OUT02TOKIEN_out02tokien_SHIFT                                     0
#define     OUT02TOKIEN_out02tokien_MASK                                      (0x7<<0)

#define     IVECT_Iv_e                                                        7
#define     IVECT_Iv_SHIFT                                                    1
#define     IVECT_Iv_MASK                                                     (0x7F<<1)

#define     EPRST_Fiforst                                                     6
#define     EPRST_togrst                                                      5
#define     EPRST_Io_hcio                                                     4
#define     EPRST_Epnum_e                                                     3
#define     EPRST_Epnum_SHIFT                                                 0
#define     EPRST_Epnum_MASK                                                  (0xF<<0)

#define     UsbCTRL_STUS_discon                                               6
#define     UsbCTRL_STUS_sigrsume                                             5
#define     UsbCTRL_STUS_usbspeed                                             1
#define     UsbCTRL_STUS_hclsmode                                             0

#define     FrmCNTL_FrL_e                                                     7
#define     FrmCNTL_FrL_SHIFT                                                 3
#define     FrmCNTL_FrL_MASK                                                  (0x1F<<3)
#define     FrmCNTL_MfrCNT_e                                                  2
#define     FrmCNTL_MfrCNT_SHIFT                                              0
#define     FrmCNTL_MfrCNT_MASK                                               (0x7<<0)

#define     FrmCNTH_FrH_e                                                     5
#define     FrmCNTH_FrH_SHIFT                                                 0
#define     FrmCNTH_FrH_MASK                                                  (0x3F<<0)

#define     Fnaddr_Fa_e                                                       6
#define     Fnaddr_Fa_SHIFT                                                   0
#define     Fnaddr_Fa_MASK                                                    (0x7F<<0)

#define     Clkgate_gosuspend                                                 0

#define     Fifoctrl_fifoauto                                                 5
#define     Fifoctrl_Io_hcio                                                  4
#define     Fifoctrl_Epnum_e                                                  3
#define     Fifoctrl_Epnum_SHIFT                                              0
#define     Fifoctrl_Epnum_MASK                                               (0xF<<0)

#define     OUT1STADDR_OUT1staddr_e                                           15
#define     OUT1STADDR_OUT1staddr_SHIFT                                       0
#define     OUT1STADDR_OUT1staddr_MASK                                        (0xFFFF<<0)

#define     OUT2STADDR_OUT2staddr_e                                           15
#define     OUT2STADDR_OUT2staddr_SHIFT                                       0
#define     OUT2STADDR_OUT2staddr_MASK                                        (0xFFFF<<0)

#define     IN1STADDR_IN1staddr_e                                             15
#define     IN1STADDR_IN1staddr_SHIFT                                         0
#define     IN1STADDR_IN1staddr_MASK                                          (0xFFFF<<0)

#define     IN2STADDR_IN2staddr_e                                             15
#define     IN2STADDR_IN2staddr_SHIFT                                         0
#define     IN2STADDR_IN2staddr_MASK                                          (0xFFFF<<0)

#define     USBEIRQ_usbirq                                                    7
#define     USBEIRQ_wakeupirq                                                 6
#define     USBEIRQ_resumeirq                                                 5
#define     USBEIRQ_con_disconir                                              4
#define     USBEIRQ_usbien                                                    3
#define     USBEIRQ_wakeupien                                                 2
#define     USBEIRQ_resumeien                                                 1
#define     USBEIRQ_con_disconien                                             0

#define     NAKOUTCTRL_Nakout2                                                1
#define     NAKOUTCTRL_nakout1                                                0

#define     HCINCTRL_Hcin2endir                                               7
#define     HCINCTRL_Hcin1endir                                               6
#define     HCINCTRL_Hcin2endien                                              5
#define     HCINCTRL_Hcin1endien                                              4
#define     HCINCTRL_Hcin2start                                               3
#define     HCINCTRL_Hcin2shpctrl                                             2
#define     HCINCTRL_Hcin1start                                               1
#define     HCINCTRL_Hcin1shpctrl                                             0

#define     OSHRTPCKIR_Oshrtpackir2                                           6
#define     OSHRTPCKIR_Oshrtpackir1                                           5
#define     OSHRTPCKIR_Oshrtpackien2                                          2
#define     OSHRTPCKIR_oshrtpackien1                                          1

#define     USBDEBUG_Debugen                                                  4
#define     USBDEBUG_dbgmode_e                                                3
#define     USBDEBUG_dbgmode_SHIFT                                            0
#define     USBDEBUG_dbgmode_MASK                                             (0xF<<0)

#define     HCOUTEMPCON_Hcout2irq                                             5
#define     HCOUTEMPCON_Hcout1irq                                             4
#define     HCOUTEMPCON_Hcout2irqen                                           3
#define     HCOUTEMPCON_Hcout1irqen                                           2
#define     HCOUTEMPCON_Hcout2m                                               1
#define     HCOUTEMPCON_Hcout1m                                               0

#define     VDCTRL_vload                                                      4
#define     VDCTRL_vcontrol_e                                                 3
#define     VDCTRL_vcontrol_SHIFT                                             0
#define     VDCTRL_vcontrol_MASK                                              (0xF<<0)

#define     VDSTAT_vstatus_e                                                  7
#define     VDSTAT_vstatus_SHIFT                                              0
#define     VDSTAT_vstatus_MASK                                               (0xFF<<0)

#define     MEMORYCTRL_outbufmode                                             0

#define     BKDOOR_hsdisable                                                  7
#define     BKDOOR_spdrqd                                                     6
#define     BKDOOR_a_ndiscon                                                  5
#define     BKDOOR_Clk_120M_Neg                                               4
#define     BKDOOR_Hnp_b_switch                                               3
#define     BKDOOR_forcefs                                                    1
#define     BKDOOR_forcehs                                                    0

#define     DMAEPSEL_dmaepsel_e                                               3
#define     DMAEPSEL_dmaepsel_SHIFT                                           0
#define     DMAEPSEL_dmaepsel_MASK                                            (0xF<<0)

#define     OTGSTAIEN_bwtaconien                                              2
#define     OTGSTAIEN_awtbconien                                              1
#define     OTGSTAIEN_asuspien                                                0

#define     OTGSTAIR_bwtaconirq                                               2
#define     OTGSTAIR_awtbconirq                                               1
#define     OTGSTAIR_asuspirq                                                 0

#define     HCIN1_COUNTL_hcin1cntl_e                                          7
#define     HCIN1_COUNTL_hcin1cntl_SHIFT                                      0
#define     HCIN1_COUNTL_hcin1cntl_MASK                                       (0xFF<<0)

#define     HCIN1_COUNTH_hcin1cnth_e                                          7
#define     HCIN1_COUNTH_hcin1cnth_SHIFT                                      0
#define     HCIN1_COUNTH_hcin1cnth_MASK                                       (0xFF<<0)

#define     HCIN2_COUNTL_Hcin2cntl_e                                          7
#define     HCIN2_COUNTL_Hcin2cntl_SHIFT                                      0
#define     HCIN2_COUNTL_Hcin2cntl_MASK                                       (0xFF<<0)

#define     HCIN2_COUNTH_Hcin2cnth_e                                          7
#define     HCIN2_COUNTH_Hcin2cnth_SHIFT                                      0
#define     HCIN2_COUNTH_Hcin2cnth_MASK                                       (0xFF<<0)

#define     IDVBUSCTRL_SoftID                                                 3
#define     IDVBUSCTRL_SoftID_EN                                              2
#define     IDVBUSCTRL_SoftVbus                                               1
#define     IDVBUSCTRL_SoftVbus_en                                            0

#define     LINESTATUS_USB_ls_e                                               4
#define     LINESTATUS_USB_ls_SHIFT                                           3
#define     LINESTATUS_USB_ls_MASK                                            (0x3<<3)
#define     LINESTATUS_otgreset                                               0

#define     DPDMCTRL_Plugin                                                   6
#define     DPDMCTRL_LineDetEN                                                4
#define     DPDMCTRL_DMPuEn                                                   3
#define     DPDMCTRL_DPPuEn                                                   2
#define     DPDMCTRL_DMPdDis                                                  1
#define     DPDMCTRL_DPPdDis                                                  0

#define     USB_PHYCTRL_Phy_PLLEN                                             7
#define     USB_PHYCTRL_Phy_DALLUALLEN                                        6
#define     USB_PHYCTRL_SIRCV_e                                               2
#define     USB_PHYCTRL_SIRCV_SHIFT                                           0
#define     USB_PHYCTRL_SIRCV_MASK                                            (0x7<<0)

#define     DMANORMALLENL_NM_LengthL_e                                        7
#define     DMANORMALLENL_NM_LengthL_SHIFT                                    0
#define     DMANORMALLENL_NM_LengthL_MASK                                     (0xFF<<0)

#define     DMANORMALLENH_NM_LengthH_e                                        7
#define     DMANORMALLENH_NM_LengthH_SHIFT                                    0
#define     DMANORMALLENH_NM_LengthH_MASK                                     (0xFF<<0)

//--------------SPI-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SPI_BASE                                                          0xc0100000
#define     SPI_CTL                                                           (SPI_BASE+0x0000)
#define     SPI_CLKDIV                                                        (SPI_BASE+0x0004)
#define     SPI_STA                                                           (SPI_BASE+0x0008)
#define     SPI_RXDAT                                                         (SPI_BASE+0x000c)
#define     SPI_TXDAT                                                         (SPI_BASE+0x0010)
#define     SPI_TCNT                                                          (SPI_BASE+0x0014)

//--------------Bits Location------------------------------------------//
#define     SPI_CTL_AHB_DMA_SEL                                               28
#define     SPI_CTL_GM                                                        27
#define     SPI_CTL_CEB                                                       26
#define     SPI_CTL_SDT_e                                                     25
#define     SPI_CTL_SDT_SHIFT                                                 24
#define     SPI_CTL_SDT_MASK                                                  (0x3<<24)
#define     SPI_CTL_RDIC_e                                                    23
#define     SPI_CTL_RDIC_SHIFT                                                22
#define     SPI_CTL_RDIC_MASK                                                 (0x3<<22)
#define     SPI_CTL_TDIC_e                                                    21
#define     SPI_CTL_TDIC_SHIFT                                                20
#define     SPI_CTL_TDIC_MASK                                                 (0x3<<20)
#define     SPI_CTL_TWME                                                      19
#define     SPI_CTL_EN                                                        18
#define     SPI_CTL_RWC_e                                                     17
#define     SPI_CTL_RWC_SHIFT                                                 16
#define     SPI_CTL_RWC_MASK                                                  (0x3<<16)
#define     SPI_CTL_DTS                                                       15
#define     SPI_CTL_SSATEN                                                    14
#define     SPI_CTL_DM_e                                                      13
#define     SPI_CTL_DM_SHIFT                                                  12
#define     SPI_CTL_DM_MASK                                                   (0x3<<12)
#define     SPI_CTL_LBT                                                       11
#define     SPI_CTL_MS                                                        10
#define     SPI_CTL_DAWS_e                                                    9
#define     SPI_CTL_DAWS_SHIFT                                                8
#define     SPI_CTL_DAWS_MASK                                                 (0x3<<8)
#define     SPI_CTL_CPOS_e                                                    7
#define     SPI_CTL_CPOS_SHIFT                                                6
#define     SPI_CTL_CPOS_MASK                                                 (0x3<<6)
#define     SPI_CTL_LMFS                                                      5
#define     SPI_CTL_SSCO                                                      4
#define     SPI_CTL_TIEN                                                      3
#define     SPI_CTL_RIEN                                                      2
#define     SPI_CTL_TDEN                                                      1
#define     SPI_CTL_RDEN                                                      0

#define     SPI_CLKDIV_CLKDIV_e                                               9
#define     SPI_CLKDIV_CLKDIV_SHIFT                                           0
#define     SPI_CLKDIV_CLKDIV_MASK                                            (0x3FF<<0)

#define     SPI_STA_TFEM                                                      9
#define     SPI_STA_RFFU                                                      8
#define     SPI_STA_TFFU                                                      7
#define     SPI_STA_RFEM                                                      6
#define     SPI_STA_TFER                                                      5
#define     SPI_STA_RFER                                                      4
#define     SPI_STA_BEB                                                       3
#define     SPI_STA_TCOM                                                      2
#define     SPI_STA_TIP                                                       1
#define     SPI_STA_PIP                                                       0

#define     SPI_RXDAT_RXDAT_e                                                 31
#define     SPI_RXDAT_RXDAT_SHIFT                                             0
#define     SPI_RXDAT_RXDAT_MASK                                              (0xFFFFFFFF<<0)

#define     SPI_TXDAT_TXDAT_e                                                 31
#define     SPI_TXDAT_TXDAT_SHIFT                                             0
#define     SPI_TXDAT_TXDAT_MASK                                              (0xFFFFFFFF<<0)

#define     SPI_TCNT_TCNT_e                                                   11
#define     SPI_TCNT_TCNT_SHIFT                                               0
#define     SPI_TCNT_TCNT_MASK                                                (0xFFF<<0)

//--------------SPI_BOOT-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SPI_BOOT_BASE                                                     0xc0110000
#define     SPIBT_CTL                                                         (SPI_BOOT_BASE+0x0000)

//--------------Bits Location------------------------------------------//
#define     SPIBT_CTL_SPIBT_DBG_e                                             16
#define     SPIBT_CTL_SPIBT_DBG_SHIFT                                         14
#define     SPIBT_CTL_SPIBT_DBG_MASK                                          (0x7<<14)
#define     SPIBT_CTL_ROM5_SEL                                                13
#define     SPIBT_CTL_ROM4_SEL                                                12
#define     SPIBT_CTL_ROM3_SEL                                                11
#define     SPIBT_CTL_ROM2_SEL                                                10
#define     SPIBT_CTL_ROM1_SEL                                                9
#define     SPIBT_CTL_ROM0_SEL                                                8
#define     SPIBT_CTL_SPI_DELAY_e                                             5
#define     SPIBT_CTL_SPI_DELAY_SHIFT                                         4
#define     SPIBT_CTL_SPI_DELAY_MASK                                          (0x3<<4)
#define     SPIBT_CTL_SPIBT_MODE                                              3
#define     SPIBT_CTL_SPIBT_CLKDIV_e                                          2
#define     SPIBT_CTL_SPIBT_CLKDIV_SHIFT                                      1
#define     SPIBT_CTL_SPIBT_CLKDIV_MASK                                       (0x3<<1)
#define     SPIBT_CTL_SPIBT_EN                                                0

//--------------I2C-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     I2C_BASE                                                          0xc0130000
#define     I2C_CTL                                                           (I2C_BASE+0x0000)
#define     I2C_STAT                                                          (I2C_BASE+0x0004)
#define     I2C_ADDR                                                          (I2C_BASE+0x0008)
#define     I2C_DAT                                                           (I2C_BASE+0x000c)

//--------------Bits Location------------------------------------------//
#define     I2C_CTL_EN                                                        7
#define     I2C_CTL_MS                                                        6
#define     I2C_CTL_CLKSEL                                                    5
#define     I2C_CTL_IRQE                                                      4
#define     I2C_CTL_GBCC_e                                                    3
#define     I2C_CTL_GBCC_SHIFT                                                2
#define     I2C_CTL_GBCC_MASK                                                 (0x3<<2)
#define     I2C_CTL_RB                                                        1
#define     I2C_CTL_GACK                                                      0

#define     I2C_STAT_TCB                                                      8
#define     I2C_STAT_STPD                                                     7
#define     I2C_STAT_STAD                                                     6
#define     I2C_STAT_RWST                                                     5
#define     I2C_STAT_LBST                                                     4
#define     I2C_STAT_IRQP                                                     3
#define     I2C_STAT_BBB                                                      2
#define     I2C_STAT_BEB                                                      1
#define     I2C_STAT_RACK                                                     0

#define     I2C_ADDR_SDAD_e                                                   7
#define     I2C_ADDR_SDAD_SHIFT                                               1
#define     I2C_ADDR_SDAD_MASK                                                (0x7F<<1)

#define     I2C_DAT_TXRXDAT_e                                                 7
#define     I2C_DAT_TXRXDAT_SHIFT                                             0
#define     I2C_DAT_TXRXDAT_MASK                                              (0xFF<<0)

//--------------UART_IR-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     UART_IR_BASE                                                      0xc00f0000
#define     UART_CTL                                                          (UART_IR_BASE+0x0000)
#define     UART_RXDAT                                                        (UART_IR_BASE+0x0004)
#define     UART_TXDAT                                                        (UART_IR_BASE+0x0008)
#define     UART_STA                                                          (UART_IR_BASE+0x000c)
#define     IRC_CTL                                                           (UART_IR_BASE+0x0050)
#define     IRC_STA                                                           (UART_IR_BASE+0x0054)
#define     IRC_CC                                                            (UART_IR_BASE+0x0058)
#define     IRC_KDC                                                           (UART_IR_BASE+0x005C)
#define     IRC_CONFIG0                                                       (UART_IR_BASE+0x0060)
#define     IRC_CONFIG1                                                       (UART_IR_BASE+0x0064)

//--------------Bits Location------------------------------------------//
#define     UART_CTL_BRDIV_e                                                  31
#define     UART_CTL_BRDIV_SHIFT                                              24
#define     UART_CTL_BRDIV_MASK                                               (0xFF<<24)
#define     UART_CTL_STDBR_e                                                  23
#define     UART_CTL_STDBR_SHIFT                                              22
#define     UART_CTL_STDBR_MASK                                               (0x3<<22)
#define     UART_CTL_AHB_DMA_SEL                                              21
#define     UART_CTL_LBEN                                                     20
#define     UART_CTL_TXIE                                                     19
#define     UART_CTL_RXIE                                                     18
#define     UART_CTL_TXDE                                                     17
#define     UART_CTL_RXDE                                                     16
#define     UART_CTL_EN                                                       15
#define     UART_CTL_RTSE                                                     13
#define     UART_CTL_AFE                                                      12
#define     UART_CTL_RDIC_e                                                   11
#define     UART_CTL_RDIC_SHIFT                                               10
#define     UART_CTL_RDIC_MASK                                                (0x3<<10)
#define     UART_CTL_TDIC_e                                                   9
#define     UART_CTL_TDIC_SHIFT                                               8
#define     UART_CTL_TDIC_MASK                                                (0x3<<8)
#define     UART_CTL_MS                                                       7
#define     UART_CTL_PRS_e                                                    6
#define     UART_CTL_PRS_SHIFT                                                4
#define     UART_CTL_PRS_MASK                                                 (0x7<<4)
#define     UART_CTL_STPS                                                     2
#define     UART_CTL_DWLS_e                                                   1
#define     UART_CTL_DWLS_SHIFT                                               0
#define     UART_CTL_DWLS_MASK                                                (0x3<<0)

#define     UART_RXDAT_RXDAT_e                                                7
#define     UART_RXDAT_RXDAT_SHIFT                                            0
#define     UART_RXDAT_RXDAT_MASK                                             (0xFF<<0)

#define     UART_TXDAT_TXDAT_e                                                7
#define     UART_TXDAT_TXDAT_SHIFT                                            0
#define     UART_TXDAT_TXDAT_MASK                                             (0xFF<<0)

#define     UART_STA_UTBB                                                     21
#define     UART_STA_TXFL_e                                                   20
#define     UART_STA_TXFL_SHIFT                                               16
#define     UART_STA_TXFL_MASK                                                (0x1F<<16)
#define     UART_STA_RXFL_e                                                   15
#define     UART_STA_RXFL_SHIFT                                               11
#define     UART_STA_RXFL_MASK                                                (0x1F<<11)
#define     UART_STA_TFES                                                     10
#define     UART_STA_RFFS                                                     9
#define     UART_STA_RTSS                                                     8
#define     UART_STA_CTSS                                                     7
#define     UART_STA_TFFU                                                     6
#define     UART_STA_RFEM                                                     5
#define     UART_STA_RXST                                                     4
#define     UART_STA_TFER                                                     3
#define     UART_STA_RXER                                                     2
#define     UART_STA_TIP                                                      1
#define     UART_STA_RIP                                                      0

#define     IRC_CTL_IRSS                                                      9
#define     IRC_CTL_TST_e                                                     8
#define     IRC_CTL_TST_SHIFT                                                 7
#define     IRC_CTL_TST_MASK                                                  (0x3<<7)
#define     IRC_CTL_TSCC_e                                                    6
#define     IRC_CTL_TSCC_SHIFT                                                5
#define     IRC_CTL_TSCC_MASK                                                 (0x3<<5)
#define     IRC_CTL_TB                                                        4
#define     IRC_CTL_IRE                                                       3
#define     IRC_CTL_IIE                                                       2
#define     IRC_CTL_ICMS_e                                                    1
#define     IRC_CTL_ICMS_SHIFT                                                0
#define     IRC_CTL_ICMS_MASK                                                 (0x3<<0)

#define     IRC_STA_TR                                                        9
#define     IRC_STA_TCB                                                       8
#define     IRC_STA_UCMP                                                      6
#define     IRC_STA_KDCM                                                      5
#define     IRC_STA_RCD                                                       4
#define     IRC_STA_IIP                                                       2
#define     IRC_STA_IREP                                                      0

#define     IRC_CC_ICCC_e                                                     15
#define     IRC_CC_ICCC_SHIFT                                                 0
#define     IRC_CC_ICCC_MASK                                                  (0xFFFF<<0)

#define     IRC_KDC_IKDC_e                                                    15
#define     IRC_KDC_IKDC_SHIFT                                                0
#define     IRC_KDC_IKDC_MASK                                                 (0xFFFF<<0)

#define     IRC_CONFIG0_RG_INTNH_e                                            31
#define     IRC_CONFIG0_RG_INTNH_SHIFT                                        29
#define     IRC_CONFIG0_RG_INTNH_MASK                                         (0x7<<29)
#define     IRC_CONFIG0_RG_INTPH_e                                            28
#define     IRC_CONFIG0_RG_INTPH_SHIFT                                        26
#define     IRC_CONFIG0_RG_INTPH_MASK                                         (0x7<<26)
#define     IRC_CONFIG0_RG_TSTH                                               25
#define     IRC_CONFIG0_RG_CMPDFH                                             24
#define     IRC_CONFIG0_RG_ENTUNEH                                            23
#define     IRC_CONFIG0_RG_RCALH_e                                            22
#define     IRC_CONFIG0_RG_RCALH_SHIFT                                        17
#define     IRC_CONFIG0_RG_RCALH_MASK                                         (0x3F<<17)
#define     IRC_CONFIG0_RG_RCCH                                               16
#define     IRC_CONFIG0_RG_DQH_e                                              15
#define     IRC_CONFIG0_RG_DQH_SHIFT                                          12
#define     IRC_CONFIG0_RG_DQH_MASK                                           (0xF<<12)
#define     IRC_CONFIG0_RG_DQC                                                11
//#define     IRC_CONFIG0_RG_DGH                                                10£º5
#define     IRC_CONFIG0_RG_DGC                                                4
//#define     IRC_CONFIG0_RG_IVRH                                               3£º1
#define     IRC_CONFIG0_ENH                                                   0

#define     IRC_CONFIG1_DGH_e                                                 31
#define     IRC_CONFIG1_DGH_SHIFT                                             26
#define     IRC_CONFIG1_DGH_MASK                                              (0x3F<<26)
#define     IRC_CONFIG1_DQH_e                                                 25
#define     IRC_CONFIG1_DQH_SHIFT                                             22
#define     IRC_CONFIG1_DQH_MASK                                              (0xF<<22)
#define     IRC_CONFIG1_RG_RC_e                                               21
#define     IRC_CONFIG1_RG_RC_SHIFT                                           16
#define     IRC_CONFIG1_RG_RC_MASK                                            (0x3F<<16)
#define     IRC_CONFIG1_RG_DIO                                                8
#define     IRC_CONFIG1_RG_DETVH                                              7
#define     IRC_CONFIG1_CFS_e                                                 6
#define     IRC_CONFIG1_CFS_SHIFT                                             5
#define     IRC_CONFIG1_CFS_MASK                                              (0x3<<5)
#define     IRC_CONFIG1_RG_TSTBSH                                             4
#define     IRC_CONFIG1_RG_CMPBIAS                                            3
#define     IRC_CONFIG1_RG_CMPVMH                                             2
#define     IRC_CONFIG1_RG_CMPVC                                              1
#define     IRC_CONFIG1_ENHYH                                                 0

//--------------AES-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     AES_BASE                                                          0xc0030000
#define     AES_CTL                                                           (AES_BASE+0x0000)
#define     AES_DIN0                                                          (AES_BASE+0x0004)
#define     AES_DIN1                                                          (AES_BASE+0x0008)
#define     AES_DIN2                                                          (AES_BASE+0x000c)
#define     AES_DIN3                                                          (AES_BASE+0x0010)
#define     AES_DOUT0                                                         (AES_BASE+0x0014)
#define     AES_DOUT1                                                         (AES_BASE+0x0018)
#define     AES_DOUT2                                                         (AES_BASE+0x001c)
#define     AES_DOUT3                                                         (AES_BASE+0x0020)
#define     AES_KEY0                                                          (AES_BASE+0x0024)
#define     AES_KEY1                                                          (AES_BASE+0x0028)
#define     AES_KEY2                                                          (AES_BASE+0x002c)
#define     AES_KEY3                                                          (AES_BASE+0x0030)

//--------------Bits Location------------------------------------------//
#define     AES_CTL_AES_KSEL                                                  6
#define     AES_CTL_AES_READY                                                 5
#define     AES_CTL_AES_CMP                                                   4
#define     AES_CTL_AES_RST                                                   3
#define     AES_CTL_EN_DECRYPT                                                2
#define     AES_CTL_AES_EN                                                    0

#define     AES_DIN0_AES_WORD0_e                                              31
#define     AES_DIN0_AES_WORD0_SHIFT                                          0
#define     AES_DIN0_AES_WORD0_MASK                                           (0xFFFFFFFF<<0)

#define     AES_DIN1_AES_WORD1_e                                              31
#define     AES_DIN1_AES_WORD1_SHIFT                                          0
#define     AES_DIN1_AES_WORD1_MASK                                           (0xFFFFFFFF<<0)

#define     AES_DIN2_AES_WORD2_e                                              31
#define     AES_DIN2_AES_WORD2_SHIFT                                          0
#define     AES_DIN2_AES_WORD2_MASK                                           (0xFFFFFFFF<<0)

#define     AES_DIN3_AES_WORD3_e                                              31
#define     AES_DIN3_AES_WORD3_SHIFT                                          0
#define     AES_DIN3_AES_WORD3_MASK                                           (0xFFFFFFFF<<0)

#define     AES_DOUT0_AES_WORD0_e                                             31
#define     AES_DOUT0_AES_WORD0_SHIFT                                         0
#define     AES_DOUT0_AES_WORD0_MASK                                          (0xFFFFFFFF<<0)

#define     AES_DOUT1_AES_WORD1_e                                             31
#define     AES_DOUT1_AES_WORD1_SHIFT                                         0
#define     AES_DOUT1_AES_WORD1_MASK                                          (0xFFFFFFFF<<0)

#define     AES_DOUT2_AES_WORD2_e                                             31
#define     AES_DOUT2_AES_WORD2_SHIFT                                         0
#define     AES_DOUT2_AES_WORD2_MASK                                          (0xFFFFFFFF<<0)

#define     AES_DOUT3_AES_WORD3_e                                             31
#define     AES_DOUT3_AES_WORD3_SHIFT                                         0
#define     AES_DOUT3_AES_WORD3_MASK                                          (0xFFFFFFFF<<0)

#define     AES_KEY0_KEY_WORD0_e                                              31
#define     AES_KEY0_KEY_WORD0_SHIFT                                          0
#define     AES_KEY0_KEY_WORD0_MASK                                           (0xFFFFFFFF<<0)

#define     AES_KEY1_KEY_WORD1_e                                              31
#define     AES_KEY1_KEY_WORD1_SHIFT                                          0
#define     AES_KEY1_KEY_WORD1_MASK                                           (0xFFFFFFFF<<0)

#define     AES_KEY2_KEY_WORD2_e                                              31
#define     AES_KEY2_KEY_WORD2_SHIFT                                          0
#define     AES_KEY2_KEY_WORD2_MASK                                           (0xFFFFFFFF<<0)

#define     AES_KEY3_KEY_WORD3_e                                              31
#define     AES_KEY3_KEY_WORD3_SHIFT                                          0
#define     AES_KEY3_KEY_WORD3_MASK                                           (0xFFFFFFFF<<0)

//--------------LCDC-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     LCDC_BASE                                                         0xc00d0000
#define     LCD_CTL                                                           (LCDC_BASE+0x0000)
#define     LCD_CLKCTL                                                        (LCDC_BASE+0x0004)
#define     EXTMEM_CTL                                                        (LCDC_BASE+0x0008)
#define     EXTMEM_CLKCTL                                                     (LCDC_BASE+0x000c)
#define     EXTMEM_DATA                                                       (LCDC_BASE+0x0010)
#define     LCD_IF_PCS                                                        (LCDC_BASE+0x0014)

//--------------Bits Location------------------------------------------//
#define     LCD_CTL_LCDFI                                                     31
#define     LCD_CTL_PC_EN                                                     30
#define     LCD_CTL_FOVF                                                      17
#define     LCD_CTL_FIFOET                                                    10
#define     LCD_CTL_EMDE                                                      7
#define     LCD_CTL_FORMATS                                                   4
#define     LCD_CTL_SEQ                                                       3
#define     LCD_CTL_SDT                                                       1
#define     LCD_CTL_EN                                                        0

#define     LCD_CLKCTL_CLKHDU_e                                               11
#define     LCD_CLKCTL_CLKHDU_SHIFT                                           8
#define     LCD_CLKCTL_CLKHDU_MASK                                            (0xF<<8)
#define     LCD_CLKCTL_CLKLDU_e                                               3
#define     LCD_CLKCTL_CLKLDU_SHIFT                                           0
#define     LCD_CLKCTL_CLKLDU_MASK                                            (0xF<<0)

#define     EXTMEM_CTL_CESEL_e                                                31
#define     EXTMEM_CTL_CESEL_SHIFT                                            29
#define     EXTMEM_CTL_CESEL_MASK                                             (0x7<<29)
#define     EXTMEM_CTL_IFSEL                                                  8
#define     EXTMEM_CTL_RS                                                     0

#define     EXTMEM_CLKCTL_EXCLKH_e                                            11
#define     EXTMEM_CLKCTL_EXCLKH_SHIFT                                        8
#define     EXTMEM_CLKCTL_EXCLKH_MASK                                         (0xF<<8)
#define     EXTMEM_CLKCTL_EXCLKL_e                                            3
#define     EXTMEM_CLKCTL_EXCLKL_SHIFT                                        0
#define     EXTMEM_CLKCTL_EXCLKL_MASK                                         (0xF<<0)

#define     EXTMEM_DATA_EXT_DATA_e                                            7
#define     EXTMEM_DATA_EXT_DATA_SHIFT                                        0
#define     EXTMEM_DATA_EXT_DATA_MASK                                         (0xFF<<0)

#define     LCD_IF_PCS_PCS_e                                                  7
#define     LCD_IF_PCS_PCS_SHIFT                                              0
#define     LCD_IF_PCS_PCS_MASK                                               (0xFF<<0)

//--------------SEG_SREEN-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     SEG_SREEN_BASE                                                    0xc00e0000
#define     SEG_SREEN_CTL                                                     (SEG_SREEN_BASE+0x00)
#define     SEG_SREEN_DATA0                                                   (SEG_SREEN_BASE+0x04)
#define     SEG_SREEN_DATA1                                                   (SEG_SREEN_BASE+0x08)
#define     SEG_SREEN_DATA2                                                   (SEG_SREEN_BASE+0x0C)
#define     SEG_SREEN_DATA3                                                   (SEG_SREEN_BASE+0x10)

//--------------Bits Location------------------------------------------//
#define     SEG_SREEN_CTL_LCD_POWER                                           31
#define     SEG_SREEN_CTL_LCD_CLK_DIV                                         8
#define     SEG_SREEN_CTL_SEGOFF                                              7
#define     SEG_SREEN_CTL_REFRSH                                              3
#define     SEG_SREEN_CTL_MODE_SEL_e                                          2
#define     SEG_SREEN_CTL_MODE_SEL_SHIFT                                      0
#define     SEG_SREEN_CTL_MODE_SEL_MASK                                       (0x7<<0)

#define     SEG_SREEN_DATA0_SEG7_COM0_e                                       31
#define     SEG_SREEN_DATA0_SEG7_COM0_SHIFT                                   28
#define     SEG_SREEN_DATA0_SEG7_COM0_MASK                                    (0xF<<28)
#define     SEG_SREEN_DATA0_SEG6_COM0_e                                       27
#define     SEG_SREEN_DATA0_SEG6_COM0_SHIFT                                   24
#define     SEG_SREEN_DATA0_SEG6_COM0_MASK                                    (0xF<<24)
#define     SEG_SREEN_DATA0_SEG5_COM0_e                                       23
#define     SEG_SREEN_DATA0_SEG5_COM0_SHIFT                                   20
#define     SEG_SREEN_DATA0_SEG5_COM0_MASK                                    (0xF<<20)
#define     SEG_SREEN_DATA0_SEG4_COM0_e                                       19
#define     SEG_SREEN_DATA0_SEG4_COM0_SHIFT                                   16
#define     SEG_SREEN_DATA0_SEG4_COM0_MASK                                    (0xF<<16)
#define     SEG_SREEN_DATA0_SEG3_COM0_e                                       15
#define     SEG_SREEN_DATA0_SEG3_COM0_SHIFT                                   12
#define     SEG_SREEN_DATA0_SEG3_COM0_MASK                                    (0xF<<12)
#define     SEG_SREEN_DATA0_SEG2_COM0_e                                       11
#define     SEG_SREEN_DATA0_SEG2_COM0_SHIFT                                   8
#define     SEG_SREEN_DATA0_SEG2_COM0_MASK                                    (0xF<<8)
#define     SEG_SREEN_DATA0_SEG1_COM0_e                                       7
#define     SEG_SREEN_DATA0_SEG1_COM0_SHIFT                                   4
#define     SEG_SREEN_DATA0_SEG1_COM0_MASK                                    (0xF<<4)
#define     SEG_SREEN_DATA0_SEG0_COM0_e                                       3
#define     SEG_SREEN_DATA0_SEG0_COM0_SHIFT                                   0
#define     SEG_SREEN_DATA0_SEG0_COM0_MASK                                    (0xF<<0)

#define     SEG_SREEN_DATA1_SEG15_COM0_e                                      31
#define     SEG_SREEN_DATA1_SEG15_COM0_SHIFT                                  28
#define     SEG_SREEN_DATA1_SEG15_COM0_MASK                                   (0xF<<28)
#define     SEG_SREEN_DATA1_SEG14_COM0_e                                      27
#define     SEG_SREEN_DATA1_SEG14_COM0_SHIFT                                  24
#define     SEG_SREEN_DATA1_SEG14_COM0_MASK                                   (0xF<<24)
#define     SEG_SREEN_DATA1_SEG13_COM0_e                                      23
#define     SEG_SREEN_DATA1_SEG13_COM0_SHIFT                                  20
#define     SEG_SREEN_DATA1_SEG13_COM0_MASK                                   (0xF<<20)
#define     SEG_SREEN_DATA1_SEG12_COM0_e                                      19
#define     SEG_SREEN_DATA1_SEG12_COM0_SHIFT                                  16
#define     SEG_SREEN_DATA1_SEG12_COM0_MASK                                   (0xF<<16)
#define     SEG_SREEN_DATA1_SEG11_COM0_e                                      15
#define     SEG_SREEN_DATA1_SEG11_COM0_SHIFT                                  12
#define     SEG_SREEN_DATA1_SEG11_COM0_MASK                                   (0xF<<12)
#define     SEG_SREEN_DATA1_SEG10_COM0_e                                      11
#define     SEG_SREEN_DATA1_SEG10_COM0_SHIFT                                  8
#define     SEG_SREEN_DATA1_SEG10_COM0_MASK                                   (0xF<<8)
#define     SEG_SREEN_DATA1_SEG9_COM0_e                                       7
#define     SEG_SREEN_DATA1_SEG9_COM0_SHIFT                                   4
#define     SEG_SREEN_DATA1_SEG9_COM0_MASK                                    (0xF<<4)
#define     SEG_SREEN_DATA1_SEG8_COM0_e                                       3
#define     SEG_SREEN_DATA1_SEG8_COM0_SHIFT                                   0
#define     SEG_SREEN_DATA1_SEG8_COM0_MASK                                    (0xF<<0)

#define     SEG_SREEN_DATA2_SEG23_COM0_e                                      31
#define     SEG_SREEN_DATA2_SEG23_COM0_SHIFT                                  28
#define     SEG_SREEN_DATA2_SEG23_COM0_MASK                                   (0xF<<28)
#define     SEG_SREEN_DATA2_SEG22_COM0_e                                      27
#define     SEG_SREEN_DATA2_SEG22_COM0_SHIFT                                  24
#define     SEG_SREEN_DATA2_SEG22_COM0_MASK                                   (0xF<<24)
#define     SEG_SREEN_DATA2_SEG21_COM0_e                                      23
#define     SEG_SREEN_DATA2_SEG21_COM0_SHIFT                                  20
#define     SEG_SREEN_DATA2_SEG21_COM0_MASK                                   (0xF<<20)
#define     SEG_SREEN_DATA2_SEG20_COM0_e                                      19
#define     SEG_SREEN_DATA2_SEG20_COM0_SHIFT                                  16
#define     SEG_SREEN_DATA2_SEG20_COM0_MASK                                   (0xF<<16)
#define     SEG_SREEN_DATA2_SEG19_COM0_e                                      15
#define     SEG_SREEN_DATA2_SEG19_COM0_SHIFT                                  12
#define     SEG_SREEN_DATA2_SEG19_COM0_MASK                                   (0xF<<12)
#define     SEG_SREEN_DATA2_SEG18_COM0_e                                      11
#define     SEG_SREEN_DATA2_SEG18_COM0_SHIFT                                  8
#define     SEG_SREEN_DATA2_SEG18_COM0_MASK                                   (0xF<<8)
#define     SEG_SREEN_DATA2_SEG17_COM0_e                                      7
#define     SEG_SREEN_DATA2_SEG17_COM0_SHIFT                                  4
#define     SEG_SREEN_DATA2_SEG17_COM0_MASK                                   (0xF<<4)
#define     SEG_SREEN_DATA2_SEG16_COM0_e                                      3
#define     SEG_SREEN_DATA2_SEG16_COM0_SHIFT                                  0
#define     SEG_SREEN_DATA2_SEG16_COM0_MASK                                   (0xF<<0)

#define     SEG_SREEN_DATA3_SEG31_COM0_e                                      31
#define     SEG_SREEN_DATA3_SEG31_COM0_SHIFT                                  28
#define     SEG_SREEN_DATA3_SEG31_COM0_MASK                                   (0xF<<28)
#define     SEG_SREEN_DATA3_SEG30_COM0_e                                      27
#define     SEG_SREEN_DATA3_SEG30_COM0_SHIFT                                  24
#define     SEG_SREEN_DATA3_SEG30_COM0_MASK                                   (0xF<<24)
#define     SEG_SREEN_DATA3_SEG29_COM0_e                                      23
#define     SEG_SREEN_DATA3_SEG29_COM0_SHIFT                                  20
#define     SEG_SREEN_DATA3_SEG29_COM0_MASK                                   (0xF<<20)
#define     SEG_SREEN_DATA3_SEG28_COM0_e                                      19
#define     SEG_SREEN_DATA3_SEG28_COM0_SHIFT                                  16
#define     SEG_SREEN_DATA3_SEG28_COM0_MASK                                   (0xF<<16)
#define     SEG_SREEN_DATA3_SEG27_COM0_e                                      15
#define     SEG_SREEN_DATA3_SEG27_COM0_SHIFT                                  12
#define     SEG_SREEN_DATA3_SEG27_COM0_MASK                                   (0xF<<12)
#define     SEG_SREEN_DATA3_SEG26_COM0_e                                      11
#define     SEG_SREEN_DATA3_SEG26_COM0_SHIFT                                  8
#define     SEG_SREEN_DATA3_SEG26_COM0_MASK                                   (0xF<<8)
#define     SEG_SREEN_DATA3_SEG25_COM0_e                                      7
#define     SEG_SREEN_DATA3_SEG25_COM0_SHIFT                                  4
#define     SEG_SREEN_DATA3_SEG25_COM0_MASK                                   (0xF<<4)
#define     SEG_SREEN_DATA3_SEG24_COM0_e                                      3
#define     SEG_SREEN_DATA3_SEG24_COM0_SHIFT                                  0
#define     SEG_SREEN_DATA3_SEG24_COM0_MASK                                   (0xF<<0)

//--------------TKC-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     TKC_BASE                                                          0xc0040000
#define     TK_CTL                                                            (TKC_BASE+0x0000)
#define     TK_STATUS                                                         (TKC_BASE+0x0004)
#define     TK_PRM                                                            (TKC_BASE+0x0008)
#define     TK_DEBUG0                                                         (TKC_BASE+0x000c)
#define     TK_DEBUG1                                                         (TKC_BASE+0x0010)
#define     TK_DEBUG2                                                         (TKC_BASE+0x0014)
#define     TK_DEBUG3                                                         (TKC_BASE+0x0018)
#define     TK_DEBUG4                                                         (TKC_BASE+0x001c)

//--------------Bits Location------------------------------------------//
#define     TK_CTL_TKEN                                                       31
#define     TK_CTL_K4EN                                                       8
#define     TK_CTL_K3EN                                                       7
#define     TK_CTL_K2EN                                                       6
#define     TK_CTL_K1EN                                                       5
#define     TK_CTL_K0EN                                                       4
#define     TK_CTL_IDA_ADJUST_EN                                              3
#define     TK_CTL_TK_INT_EN                                                  1
#define     TK_CTL_TK_LOCK                                                    0

#define     TK_STATUS_INT4PD                                                  31
#define     TK_STATUS_INT3PD                                                  30
#define     TK_STATUS_INT2PD                                                  29
#define     TK_STATUS_INT1PD                                                  28
#define     TK_STATUS_INT0PD                                                  27
#define     TK_STATUS_K4ST                                                    16
#define     TK_STATUS_K3ST                                                    12
#define     TK_STATUS_K2ST                                                    8
#define     TK_STATUS_K1ST                                                    4
#define     TK_STATUS_K0ST                                                    0

#define     TK_PRM_DB_NUM_e                                                   29
#define     TK_PRM_DB_NUM_SHIFT                                               28
#define     TK_PRM_DB_NUM_MASK                                                (0x3<<28)
#define     TK_PRM_TK_TH_e                                                    27
#define     TK_PRM_TK_TH_SHIFT                                                24
#define     TK_PRM_TK_TH_MASK                                                 (0xF<<24)
#define     TK_PRM_BL_CAL_TH_e                                                23
#define     TK_PRM_BL_CAL_TH_SHIFT                                            20
#define     TK_PRM_BL_CAL_TH_MASK                                             (0xF<<20)
#define     TK_PRM_IDAC_e                                                     15
#define     TK_PRM_IDAC_SHIFT                                                 12
#define     TK_PRM_IDAC_MASK                                                  (0xF<<12)
#define     TK_PRM_NOISE_TH_e                                                 10
#define     TK_PRM_NOISE_TH_SHIFT                                             8
#define     TK_PRM_NOISE_TH_MASK                                              (0x7<<8)
#define     TK_PRM_BUCKET_TH_e                                                7
#define     TK_PRM_BUCKET_TH_SHIFT                                            5
#define     TK_PRM_BUCKET_TH_MASK                                             (0x7<<5)
#define     TK_PRM_BL_STEP_e                                                  4
#define     TK_PRM_BL_STEP_SHIFT                                              2
#define     TK_PRM_BL_STEP_MASK                                               (0x7<<2)

#define     TK_DEBUG0_RAW_COUNT1_e                                            31
#define     TK_DEBUG0_RAW_COUNT1_SHIFT                                        19
#define     TK_DEBUG0_RAW_COUNT1_MASK                                         (0x1FFF<<19)
#define     TK_DEBUG0_BASELINE1_e                                             18
#define     TK_DEBUG0_BASELINE1_SHIFT                                         6
#define     TK_DEBUG0_BASELINE1_MASK                                          (0x1FFF<<6)

#define     TK_DEBUG1_RAW_COUNT2_e                                            31
#define     TK_DEBUG1_RAW_COUNT2_SHIFT                                        19
#define     TK_DEBUG1_RAW_COUNT2_MASK                                         (0x1FFF<<19)
#define     TK_DEBUG1_BASELINE2_e                                             18
#define     TK_DEBUG1_BASELINE2_SHIFT                                         6
#define     TK_DEBUG1_BASELINE2_MASK                                          (0x1FFF<<6)

#define     TK_DEBUG2_RAW_COUNT3_e                                            31
#define     TK_DEBUG2_RAW_COUNT3_SHIFT                                        19
#define     TK_DEBUG2_RAW_COUNT3_MASK                                         (0x1FFF<<19)
#define     TK_DEBUG2_BASELINE3_e                                             18
#define     TK_DEBUG2_BASELINE3_SHIFT                                         6
#define     TK_DEBUG2_BASELINE3_MASK                                          (0x1FFF<<6)

#define     TK_DEBUG3_RAW_COUNT4_e                                            31
#define     TK_DEBUG3_RAW_COUNT4_SHIFT                                        19
#define     TK_DEBUG3_RAW_COUNT4_MASK                                         (0x1FFF<<19)
#define     TK_DEBUG3_BASELINE4_e                                             18
#define     TK_DEBUG3_BASELINE4_SHIFT                                         6
#define     TK_DEBUG3_BASELINE4_MASK                                          (0x1FFF<<6)

#define     TK_DEBUG4_RAW_COUNT5_e                                            31
#define     TK_DEBUG4_RAW_COUNT5_SHIFT                                        19
#define     TK_DEBUG4_RAW_COUNT5_MASK                                         (0x1FFF<<19)
#define     TK_DEBUG4_BASELINE5_e                                             18
#define     TK_DEBUG4_BASELINE5_SHIFT                                         6
#define     TK_DEBUG4_BASELINE5_MASK                                          (0x1FFF<<6)

//--------------DAC_PA_I2S_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     DAC_PA_I2S_Register_BASE                                          0xc0050000
#define     DAC_CTL                                                           (DAC_PA_I2S_Register_BASE+0x0000)
#define     DAC_FIFOCTL                                                       (DAC_PA_I2S_Register_BASE+0x0004)
#define     DAC_DAT                                                           (DAC_PA_I2S_Register_BASE+0x0008)
#define     DAC_DEBUG                                                         (DAC_PA_I2S_Register_BASE+0x000c)
#define     DAC_ANALOG1                                                       (DAC_PA_I2S_Register_BASE+0x0010)
#define     DAC_ANALOG2                                                       (DAC_PA_I2S_Register_BASE+0x0014)
#define     KT_CTL                                                            (DAC_PA_I2S_Register_BASE+0x0018)
#define     OUT_GAIN                                                          (DAC_PA_I2S_Register_BASE+0x001c)

//--------------Bits Location------------------------------------------//
#define     DAC_CTL_DAEN                                                      26
#define     DAC_CTL_DDEN                                                      25
#define     DAC_CTL_DDCS                                                      24
#define     DAC_CTL_AAEN                                                      23
#define     DAC_CTL_FSRF                                                      21
#define     DAC_CTL_IBND_e                                                    20
#define     DAC_CTL_IBND_SHIFT                                                19
#define     DAC_CTL_IBND_MASK                                                 (0x3<<19)
#define     DAC_CTL_INSR_e                                                    18
#define     DAC_CTL_INSR_SHIFT                                                16
#define     DAC_CTL_INSR_MASK                                                 (0x7<<16)
#define     DAC_CTL_INEN                                                      15
#define     DAC_CTL_MS                                                        14
#define     DAC_CTL_LB                                                        10
#define     DAC_CTL_OEN                                                       9
#define     DAC_CTL_SRS                                                       2
#define     DAC_CTL_SDEN                                                      1
#define     DAC_CTL_EN                                                        0

#define     DAC_FIFOCTL_FEZR                                                  31
#define     DAC_FIFOCTL_OSRDA_e                                               29
#define     DAC_FIFOCTL_OSRDA_SHIFT                                           28
#define     DAC_FIFOCTL_OSRDA_MASK                                            (0x3<<28)
#define     DAC_FIFOCTL_VOLDA_e                                               27
#define     DAC_FIFOCTL_VOLDA_SHIFT                                           20
#define     DAC_FIFOCTL_VOLDA_MASK                                            (0xFF<<20)
#define     DAC_FIFOCTL_ENDITH                                                19
#define     DAC_FIFOCTL_DFAFS                                                 18
#define     DAC_FIFOCTL_DFAES                                                 17
#define     DAC_FIFOCTL_OCIEN                                                 16
#define     DAC_FIFOCTL_FOUTS                                                 14
#define     DAC_FIFOCTL_DDRF                                                  12
#define     DAC_FIFOCTL_DAIS                                                  11
#define     DAC_FIFOCTL_DDCS                                                  8
#define     DAC_FIFOCTL_FEIP                                                  7
#define     DAC_FIFOCTL_FEIE                                                  6
#define     DAC_FIFOCTL_FEDE                                                  5
#define     DAC_FIFOCTL_FUF                                                   4
#define     DAC_FIFOCTL_FINS_e                                                3
#define     DAC_FIFOCTL_FINS_SHIFT                                            1
#define     DAC_FIFOCTL_FINS_MASK                                             (0x7<<1)
#define     DAC_FIFOCTL_FIRT                                                  0

#define     DAC_DAT_DAT_e                                                     31
#define     DAC_DAT_DAT_SHIFT                                                 8
#define     DAC_DAT_DAT_MASK                                                  (0xFFFFFF<<8)

#define     DAC_DEBUG_DATDE_e                                                 31
#define     DAC_DEBUG_DATDE_SHIFT                                             8
#define     DAC_DEBUG_DATDE_MASK                                              (0xFFFFFF<<8)

#define     DAC_ANALOG1_DACEN_e                                               31
#define     DAC_ANALOG1_DACEN_SHIFT                                           30
#define     DAC_ANALOG1_DACEN_MASK                                            (0x3<<30)
#define     DAC_ANALOG1_PMP_PAEN                                              29
#define     DAC_ANALOG1_PMP_AAMUTE                                            27
#define     DAC_ANALOG1_PMP_DAMUTE                                            26
#define     DAC_ANALOG1_PAIQ_e                                                25
#define     DAC_ANALOG1_PAIQ_SHIFT                                            24
#define     DAC_ANALOG1_PAIQ_MASK                                             (0x3<<24)
#define     DAC_ANALOG1_ZERODT                                                23
#define     DAC_ANALOG1_PVCTV                                                 22
#define     DAC_ANALOG1_Volume_e                                              21
#define     DAC_ANALOG1_Volume_SHIFT                                          16
#define     DAC_ANALOG1_Volume_MASK                                           (0x3F<<16)
#define     DAC_ANALOG1_PAIB_e                                                15
#define     DAC_ANALOG1_PAIB_SHIFT                                            14
#define     DAC_ANALOG1_PAIB_MASK                                             (0x3<<14)
#define     DAC_ANALOG1_OPDAVB_e                                              13
#define     DAC_ANALOG1_OPDAVB_SHIFT                                          12
#define     DAC_ANALOG1_OPDAVB_MASK                                           (0x3<<12)
#define     DAC_ANALOG1_PAOSW                                                 11
#define     DAC_ANALOG1_OPDAIB_e                                              10
#define     DAC_ANALOG1_OPDAIB_SHIFT                                          8
#define     DAC_ANALOG1_OPDAIB_MASK                                           (0x7<<8)
#define     DAC_ANALOG1_OPDTSIB_e                                             7
#define     DAC_ANALOG1_OPDTSIB_SHIFT                                         6
#define     DAC_ANALOG1_OPDTSIB_MASK                                          (0x3<<6)
#define     DAC_ANALOG1_OPVBIB_e                                              5
#define     DAC_ANALOG1_OPVBIB_SHIFT                                          4
#define     DAC_ANALOG1_OPVBIB_MASK                                           (0x3<<4)
#define     DAC_ANALOG1_OPGIB_e                                               2
#define     DAC_ANALOG1_OPGIB_SHIFT                                           0
#define     DAC_ANALOG1_OPGIB_MASK                                            (0x7<<0)

#define     DAC_ANALOG2_OVLS                                                  29
#define     DAC_ANALOG2_ALPIDS                                                28
#define     DAC_ANALOG2_CAR_OPOEN                                             27
#define     DAC_ANALOG2_CAR_AAMUTE                                            25
#define     DAC_ANALOG2_CAR_DAMUTE                                            24
#define     DAC_ANALOG2_ZCROSS                                                23
#define     DAC_ANALOG2_VLCHDL                                                22
//#define     DAC_ANALOG2_OPCM1IB                                               21£º20
#define     DAC_ANALOG2_ALPIDEN                                               19
#define     DAC_ANALOG2_OPVROIB_e                                             18
#define     DAC_ANALOG2_OPVROIB_SHIFT                                         16
#define     DAC_ANALOG2_OPVROIB_MASK                                          (0x7<<16)
#define     DAC_ANALOG2_ATPLP2                                                15
#define     DAC_ANALOG2_CCH                                                   14
#define     DAC_ANALOG2_PAVCC_e                                               13
#define     DAC_ANALOG2_PAVCC_SHIFT                                           12
#define     DAC_ANALOG2_PAVCC_MASK                                            (0x3<<12)
//#define     DAC_ANALOG2_ICTRL                                                 11£º10
#define     DAC_ANALOG2_OSEN                                                  9
#define     DAC_ANALOG2_CDISCH                                                8
#define     DAC_ANALOG2_DDOVV                                                 5
#define     DAC_ANALOG2_OPVROEN                                               4
#define     DAC_ANALOG2_DDATPR                                                3
#define     DAC_ANALOG2_OPVROOSIB_e                                           2
#define     DAC_ANALOG2_OPVROOSIB_SHIFT                                       0
#define     DAC_ANALOG2_OPVROOSIB_MASK                                        (0x7<<0)

#define     KT_CTL_SKT                                                        12
#define     KT_CTL_KTFR_e                                                     11
#define     KT_CTL_KTFR_SHIFT                                                 10
#define     KT_CTL_KTFR_MASK                                                  (0x3<<10)
#define     KT_CTL_KTCNT_e                                                    9
#define     KT_CTL_KTCNT_SHIFT                                                8
#define     KT_CTL_KTCNT_MASK                                                 (0x3<<8)
#define     KT_CTL_KTVOLH_e                                                   7
#define     KT_CTL_KTVOLH_SHIFT                                               6
#define     KT_CTL_KTVOLH_MASK                                                (0x3<<6)
#define     KT_CTL_KTVOLL_e                                                   5
#define     KT_CTL_KTVOLL_SHIFT                                               0
#define     KT_CTL_KTVOLL_MASK                                                (0x3F<<0)

#define     OUT_GAIN_OUTRRG_e                                                 28
#define     OUT_GAIN_OUTRRG_SHIFT                                             24
#define     OUT_GAIN_OUTRRG_MASK                                              (0x1F<<24)
#define     OUT_GAIN_OUTFRG_e                                                 20
#define     OUT_GAIN_OUTFRG_SHIFT                                             16
#define     OUT_GAIN_OUTFRG_MASK                                              (0x1F<<16)
#define     OUT_GAIN_OUTRLG_e                                                 12
#define     OUT_GAIN_OUTRLG_SHIFT                                             8
#define     OUT_GAIN_OUTRLG_MASK                                              (0x1F<<8)
#define     OUT_GAIN_OUTFLG_e                                                 4
#define     OUT_GAIN_OUTFLG_SHIFT                                             0
#define     OUT_GAIN_OUTFLG_MASK                                              (0x1F<<0)

//--------------ADC_Register-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     ADC_Register_BASE                                                 0xc0051000
#define     ADC_CTL                                                           (ADC_Register_BASE+0x0000)
#define     ADC_FIFOCTL                                                       (ADC_Register_BASE+0x0004)
#define     ADC_DAT                                                           (ADC_Register_BASE+0x0008)
#define     ADC_DEBUG                                                         (ADC_Register_BASE+0x000C)
#define     ADC_ANALOG                                                        (ADC_Register_BASE+0x0010)

//--------------Bits Location------------------------------------------//
#define     ADC_CTL_VRDAEN                                                    18
#define     ADC_CTL_AMIXMT                                                    17
#define     ADC_CTL_VMIC_CTL                                                  16
#define     ADC_CTL_ZCEN                                                      15
#define     ADC_CTL_ADCINS                                                    14
#define     ADC_CTL_ANAINS_e                                                  13
#define     ADC_CTL_ANAINS_SHIFT                                              12
#define     ADC_CTL_ANAINS_MASK                                               (0x3<<12)
#define     ADC_CTL_AMIXING_e                                                 11
#define     ADC_CTL_AMIXING_SHIFT                                             9
#define     ADC_CTL_AMIXING_MASK                                              (0x7<<9)
#define     ADC_CTL_MICINKARAEN                                               8
#define     ADC_CTL_AING_e                                                    7
#define     ADC_CTL_AING_SHIFT                                                4
#define     ADC_CTL_AING_MASK                                                 (0xF<<4)
#define     ADC_CTL_AINREN                                                    3
#define     ADC_CTL_AINLEN                                                    2
#define     ADC_CTL_ADRE                                                      1
#define     ADC_CTL_ADLE                                                      0

#define     ADC_FIFOCTL_AFAFS                                                 19
#define     ADC_FIFOCTL_AFAES                                                 18
#define     ADC_FIFOCTL_VOLAD_e                                               17
#define     ADC_FIFOCTL_VOLAD_SHIFT                                           12
#define     ADC_FIFOCTL_VOLAD_MASK                                            (0x3F<<12)
#define     ADC_FIFOCTL_ADFOS                                                 11
#define     ADC_FIFOCTL_HPFEN                                                 10
#define     ADC_FIFOCTL_ADDF                                                  9
#define     ADC_FIFOCTL_ADRF                                                  8
#define     ADC_FIFOCTL_AERP                                                  7
#define     ADC_FIFOCTL_FEF                                                   6
#define     ADC_FIFOCTL_FFIP                                                  5
#define     ADC_FIFOCTL_FFIE                                                  4
#define     ADC_FIFOCTL_FFDE                                                  3
#define     ADC_FIFOCTL_ADEN                                                  2
#define     ADC_FIFOCTL_FINS                                                  1
#define     ADC_FIFOCTL_FIRT                                                  0

#define     ADC_DAT_DAT_e                                                     31
#define     ADC_DAT_DAT_SHIFT                                                 8
#define     ADC_DAT_DAT_MASK                                                  (0xFFFFFF<<8)

#define     ADC_DEBUG_DEDAT_e                                                 31
#define     ADC_DEBUG_DEDAT_SHIFT                                             11
#define     ADC_DEBUG_DEDAT_MASK                                              (0x1FFFFF<<11)

#define     ADC_ANALOG_OPAD1CS_e                                              11
#define     ADC_ANALOG_OPAD1CS_SHIFT                                          9
#define     ADC_ANALOG_OPAD1CS_MASK                                           (0x7<<9)
#define     ADC_ANALOG_OPAD2CS_e                                              8
#define     ADC_ANALOG_OPAD2CS_SHIFT                                          7
#define     ADC_ANALOG_OPAD2CS_MASK                                           (0x3<<7)
#define     ADC_ANALOG_LPFCS_e                                                6
#define     ADC_ANALOG_LPFCS_SHIFT                                            5
#define     ADC_ANALOG_LPFCS_MASK                                             (0x3<<5)
#define     ADC_ANALOG_PRECS_e                                                4
#define     ADC_ANALOG_PRECS_SHIFT                                            3
#define     ADC_ANALOG_PRECS_MASK                                             (0x3<<3)
#define     ADC_ANALOG_VRCS_e                                                 2
#define     ADC_ANALOG_VRCS_SHIFT                                             0
#define     ADC_ANALOG_VRCS_MASK                                              (0x7<<0)

//--------------GPIO_MFP_PWM-------------------------------------------//
//--------------Register Address---------------------------------------//
#define     GPIO_MFP_PWM_BASE                                                 0xc0090000
#define     GPIO_AOUTEN                                                       (GPIO_MFP_PWM_BASE+0x0000)
#define     GPIO_AINEN                                                        (GPIO_MFP_PWM_BASE+0x0004)
#define     GPIO_ADAT                                                         (GPIO_MFP_PWM_BASE+0x0008)
#define     GPIO_APUEN                                                        (GPIO_MFP_PWM_BASE+0x000C)
#define     GPIO_APDEN                                                        (GPIO_MFP_PWM_BASE+0x0010)
#define     GPIO_BOUTEN                                                       (GPIO_MFP_PWM_BASE+0x0014)
#define     GPIO_BINEN                                                        (GPIO_MFP_PWM_BASE+0x0018)
#define     GPIO_BDAT                                                         (GPIO_MFP_PWM_BASE+0x001C)
#define     GPIO_BPUEN                                                        (GPIO_MFP_PWM_BASE+0x0020)
#define     GPIO_BPDEN                                                        (GPIO_MFP_PWM_BASE+0x0024)
#define     PWM_CTL                                                           (GPIO_MFP_PWM_BASE+0x0028)
#define     MFP_CTL0                                                          (GPIO_MFP_PWM_BASE+0x002C)
#define     MFP_CTL1                                                          (GPIO_MFP_PWM_BASE+0x0030)
#define     MFP_CTL2                                                          (GPIO_MFP_PWM_BASE+0x0034)
#define     MFP_CTL3                                                          (GPIO_MFP_PWM_BASE+0x0038)
#define     BONDING_OPTION                                                    (GPIO_MFP_PWM_BASE+0x003C)
#define     PAD_PUPD                                                          (GPIO_MFP_PWM_BASE+0x0040)
#define     PAD_SMIT0                                                         (GPIO_MFP_PWM_BASE+0x0044)
#define     PAD_SMIT1                                                         (GPIO_MFP_PWM_BASE+0x0048)
#define     PAD_DRV0                                                          (GPIO_MFP_PWM_BASE+0x004C)
#define     PAD_DRV1                                                          (GPIO_MFP_PWM_BASE+0x0050)
#define     PAD_DRV2                                                          (GPIO_MFP_PWM_BASE+0x0054)
#define     PAD_DRV3                                                          (GPIO_MFP_PWM_BASE+0x0058)
#define     PAD_DEBUG0                                                        (GPIO_MFP_PWM_BASE+0x005C)
#define     PAD_DEBUG1                                                        (GPIO_MFP_PWM_BASE+0x0060)

//--------------Bits Location------------------------------------------//
#define     GPIO_AOUTEN_GPIOA_OUTEN_e                                         31
#define     GPIO_AOUTEN_GPIOA_OUTEN_SHIFT                                     0
#define     GPIO_AOUTEN_GPIOA_OUTEN_MASK                                      (0xFFFFFFFF<<0)

#define     GPIO_AINEN_GPIOA_INEN_e                                           31
#define     GPIO_AINEN_GPIOA_INEN_SHIFT                                       0
#define     GPIO_AINEN_GPIOA_INEN_MASK                                        (0xFFFFFFFF<<0)

#define     GPIO_ADAT_GPIOA_DAT_e                                             31
#define     GPIO_ADAT_GPIOA_DAT_SHIFT                                         0
#define     GPIO_ADAT_GPIOA_DAT_MASK                                          (0xFFFFFFFF<<0)

#define     GPIO_APUEN_GPIOA_PUEN_e                                           31
#define     GPIO_APUEN_GPIOA_PUEN_SHIFT                                       0
#define     GPIO_APUEN_GPIOA_PUEN_MASK                                        (0xFFFFFFFF<<0)

#define     GPIO_APDEN_GPIOA_PDEN_e                                           31
#define     GPIO_APDEN_GPIOA_PDEN_SHIFT                                       0
#define     GPIO_APDEN_GPIOA_PDEN_MASK                                        (0xFFFFFFFF<<0)

#define     GPIO_BOUTEN_GPIOB_OUTEN_e                                         23
#define     GPIO_BOUTEN_GPIOB_OUTEN_SHIFT                                     0
#define     GPIO_BOUTEN_GPIOB_OUTEN_MASK                                      (0xFFFFFF<<0)

#define     GPIO_BINEN_GPIOB_INEN_e                                           23
#define     GPIO_BINEN_GPIOB_INEN_SHIFT                                       0
#define     GPIO_BINEN_GPIOB_INEN_MASK                                        (0xFFFFFF<<0)

#define     GPIO_BDAT_GPIOB_DAT_e                                             23
#define     GPIO_BDAT_GPIOB_DAT_SHIFT                                         0
#define     GPIO_BDAT_GPIOB_DAT_MASK                                          (0xFFFFFF<<0)

#define     GPIO_BPUEN_GPIOB_PUEN_e                                           23
#define     GPIO_BPUEN_GPIOB_PUEN_SHIFT                                       0
#define     GPIO_BPUEN_GPIOB_PUEN_MASK                                        (0xFFFFFF<<0)

#define     GPIO_BPDEN_GPIOB_PDEN_e                                           23
#define     GPIO_BPDEN_GPIOB_PDEN_SHIFT                                       0
#define     GPIO_BPDEN_GPIOB_PDEN_MASK                                        (0xFFFFFF<<0)

#define     PWM_CTL_POL_SEL                                                   4
#define     PWM_CTL_DUTY_e                                                    3
#define     PWM_CTL_DUTY_SHIFT                                                0
#define     PWM_CTL_DUTY_MASK                                                 (0xF<<0)

#define     MFP_CTL0_P_GPIOA15_e                                              31
#define     MFP_CTL0_P_GPIOA15_SHIFT                                          30
#define     MFP_CTL0_P_GPIOA15_MASK                                           (0x3<<30)
#define     MFP_CTL0_P_GPIOA14_e                                              29
#define     MFP_CTL0_P_GPIOA14_SHIFT                                          28
#define     MFP_CTL0_P_GPIOA14_MASK                                           (0x3<<28)
#define     MFP_CTL0_P_GPIOA13_e                                              27
#define     MFP_CTL0_P_GPIOA13_SHIFT                                          26
#define     MFP_CTL0_P_GPIOA13_MASK                                           (0x3<<26)
#define     MFP_CTL0_P_GPIOA12_e                                              25
#define     MFP_CTL0_P_GPIOA12_SHIFT                                          23
#define     MFP_CTL0_P_GPIOA12_MASK                                           (0x7<<23)
#define     MFP_CTL0_P_GPIOA11_e                                              22
#define     MFP_CTL0_P_GPIOA11_SHIFT                                          20
#define     MFP_CTL0_P_GPIOA11_MASK                                           (0x7<<20)
#define     MFP_CTL0_P_GPIOA10_e                                              19
#define     MFP_CTL0_P_GPIOA10_SHIFT                                          18
#define     MFP_CTL0_P_GPIOA10_MASK                                           (0x3<<18)
#define     MFP_CTL0_P_GPIOA9_e                                               17
#define     MFP_CTL0_P_GPIOA9_SHIFT                                           16
#define     MFP_CTL0_P_GPIOA9_MASK                                            (0x3<<16)
#define     MFP_CTL0_P_GPIOA8_e                                               15
#define     MFP_CTL0_P_GPIOA8_SHIFT                                           14
#define     MFP_CTL0_P_GPIOA8_MASK                                            (0x3<<14)
#define     MFP_CTL0_P_GPIOA7_e                                               13
#define     MFP_CTL0_P_GPIOA7_SHIFT                                           12
#define     MFP_CTL0_P_GPIOA7_MASK                                            (0x3<<12)
#define     MFP_CTL0_P_GPIOA6_e                                               11
#define     MFP_CTL0_P_GPIOA6_SHIFT                                           9
#define     MFP_CTL0_P_GPIOA6_MASK                                            (0x7<<9)
#define     MFP_CTL0_P_GPIOA5_e                                               8
#define     MFP_CTL0_P_GPIOA5_SHIFT                                           6
#define     MFP_CTL0_P_GPIOA5_MASK                                            (0x7<<6)
#define     MFP_CTL0_P_GPIOA4                                                 5
#define     MFP_CTL0_P_GPIOA3_e                                               4
#define     MFP_CTL0_P_GPIOA3_SHIFT                                           2
#define     MFP_CTL0_P_GPIOA3_MASK                                            (0x7<<2)
#define     MFP_CTL0_P_GPIOA2_e                                               1
#define     MFP_CTL0_P_GPIOA2_SHIFT                                           0
#define     MFP_CTL0_P_GPIOA2_MASK                                            (0x3<<0)

#define     MFP_CTL1_P_GPIOA26_e                                              29
#define     MFP_CTL1_P_GPIOA26_SHIFT                                          27
#define     MFP_CTL1_P_GPIOA26_MASK                                           (0x7<<27)
#define     MFP_CTL1_P_GPIOA25_e                                              26
#define     MFP_CTL1_P_GPIOA25_SHIFT                                          24
#define     MFP_CTL1_P_GPIOA25_MASK                                           (0x7<<24)
#define     MFP_CTL1_P_GPIOA24_e                                              23
#define     MFP_CTL1_P_GPIOA24_SHIFT                                          21
#define     MFP_CTL1_P_GPIOA24_MASK                                           (0x7<<21)
#define     MFP_CTL1_P_GPIOA23_e                                              20
#define     MFP_CTL1_P_GPIOA23_SHIFT                                          18
#define     MFP_CTL1_P_GPIOA23_MASK                                           (0x7<<18)
#define     MFP_CTL1_P_GPIOA22_e                                              17
#define     MFP_CTL1_P_GPIOA22_SHIFT                                          16
#define     MFP_CTL1_P_GPIOA22_MASK                                           (0x3<<16)
#define     MFP_CTL1_P_GPIOA21_e                                              15
#define     MFP_CTL1_P_GPIOA21_SHIFT                                          13
#define     MFP_CTL1_P_GPIOA21_MASK                                           (0x7<<13)
#define     MFP_CTL1_P_GPIOA20_e                                              12
#define     MFP_CTL1_P_GPIOA20_SHIFT                                          10
#define     MFP_CTL1_P_GPIOA20_MASK                                           (0x7<<10)
#define     MFP_CTL1_P_GPIOA19                                                9
#define     MFP_CTL1_P_GPIOA18_e                                              8
#define     MFP_CTL1_P_GPIOA18_SHIFT                                          6
#define     MFP_CTL1_P_GPIOA18_MASK                                           (0x7<<6)
#define     MFP_CTL1_P_GPIOA17_e                                              5
#define     MFP_CTL1_P_GPIOA17_SHIFT                                          3
#define     MFP_CTL1_P_GPIOA17_MASK                                           (0x7<<3)
#define     MFP_CTL1_P_GPIOA16_e                                              2
#define     MFP_CTL1_P_GPIOA16_SHIFT                                          0
#define     MFP_CTL1_P_GPIOA16_MASK                                           (0x7<<0)

#define     MFP_CTL2_P_GPIOB11_e                                              30
#define     MFP_CTL2_P_GPIOB11_SHIFT                                          28
#define     MFP_CTL2_P_GPIOB11_MASK                                           (0x7<<28)
#define     MFP_CTL2_P_GPIOB10_e                                              27
#define     MFP_CTL2_P_GPIOB10_SHIFT                                          25
#define     MFP_CTL2_P_GPIOB10_MASK                                           (0x7<<25)
#define     MFP_CTL2_P_GPIOB9_e                                               24
#define     MFP_CTL2_P_GPIOB9_SHIFT                                           22
#define     MFP_CTL2_P_GPIOB9_MASK                                            (0x7<<22)
#define     MFP_CTL2_P_GPIOB8_e                                               21
#define     MFP_CTL2_P_GPIOB8_SHIFT                                           19
#define     MFP_CTL2_P_GPIOB8_MASK                                            (0x7<<19)
#define     MFP_CTL2_P_GPIOB7                                                 18
#define     MFP_CTL2_P_GPIOB6                                                 17
#define     MFP_CTL2_P_GPIOB5                                                 16
#define     MFP_CTL2_P_GPIOB4_e                                               15
#define     MFP_CTL2_P_GPIOB4_SHIFT                                           14
#define     MFP_CTL2_P_GPIOB4_MASK                                            (0x3<<14)
#define     MFP_CTL2_P_GPIOB0                                                 13
#define     MFP_CTL2_P_GPIOA31                                                12
#define     MFP_CTL2_P_GPIOA30_e                                              11
#define     MFP_CTL2_P_GPIOA30_SHIFT                                          9
#define     MFP_CTL2_P_GPIOA30_MASK                                           (0x7<<9)
#define     MFP_CTL2_P_GPIOA29_e                                              8
#define     MFP_CTL2_P_GPIOA29_SHIFT                                          6
#define     MFP_CTL2_P_GPIOA29_MASK                                           (0x7<<6)
#define     MFP_CTL2_P_GPIOA28_e                                              5
#define     MFP_CTL2_P_GPIOA28_SHIFT                                          3
#define     MFP_CTL2_P_GPIOA28_MASK                                           (0x7<<3)
#define     MFP_CTL2_P_GPIOA27_e                                              2
#define     MFP_CTL2_P_GPIOA27_SHIFT                                          0
#define     MFP_CTL2_P_GPIOA27_MASK                                           (0x7<<0)

#define     MFP_CTL3_EJTAGEN                                                  31
#define     MFP_CTL3_EJTAGMAP_e                                               30
#define     MFP_CTL3_EJTAGMAP_SHIFT                                           29
#define     MFP_CTL3_EJTAGMAP_MASK                                            (0x3<<29)
#define     MFP_CTL3_SPIBT_PIN                                                15
#define     MFP_CTL3_P_GPIOB21_e                                              14
#define     MFP_CTL3_P_GPIOB21_SHIFT                                          13
#define     MFP_CTL3_P_GPIOB21_MASK                                           (0x3<<13)
#define     MFP_CTL3_P_GPIOB15_e                                              12
#define     MFP_CTL3_P_GPIOB15_SHIFT                                          10
#define     MFP_CTL3_P_GPIOB15_MASK                                           (0x7<<10)
#define     MFP_CTL3_P_GPIOB14_e                                              9
#define     MFP_CTL3_P_GPIOB14_SHIFT                                          7
#define     MFP_CTL3_P_GPIOB14_MASK                                           (0x7<<7)
#define     MFP_CTL3_P_GPIOB13_e                                              6
#define     MFP_CTL3_P_GPIOB13_SHIFT                                          3
#define     MFP_CTL3_P_GPIOB13_MASK                                           (0xF<<3)
#define     MFP_CTL3_P_GPIOB12_e                                              2
#define     MFP_CTL3_P_GPIOB12_SHIFT                                          0
#define     MFP_CTL3_P_GPIOB12_MASK                                           (0x7<<0)

#define     BONDING_OPTION_BOOTSEQ                                            1
#define     BONDING_OPTION_CK24M                                              0

#define     PAD_PUPD_NAND_DQS_e                                               11
#define     PAD_PUPD_NAND_DQS_SHIFT                                           10
#define     PAD_PUPD_NAND_DQS_MASK                                            (0x3<<10)
#define     PAD_PUPD_NAND_HOLD                                                9
#define     PAD_PUPD_SD_CMD                                                   8
#define     PAD_PUPD_SD_DATA                                                  7
#define     PAD_PUPD_SIRQ1PD                                                  6
#define     PAD_PUPD_SIRQ1PU                                                  5
#define     PAD_PUPD_SIRQ0_PD                                                 4
#define     PAD_PUPD_SIRQ0_PU                                                 3
#define     PAD_PUPD_NAND_RB                                                  2
#define     PAD_PUPD_I2C                                                      1
#define     PAD_PUPD_UART_RX                                                  0

#define     PAD_SMIT0_P_GPIOB10                                               31
#define     PAD_SMIT0_P_GPIOB20                                               30
#define     PAD_SMIT0_P_GPIOA16                                               29
#define     PAD_SMIT0_P_GPIOA26                                               28
#define     PAD_SMIT0_P_GPIOB11                                               27
#define     PAD_SMIT0_P_GPIOB19                                               26
#define     PAD_SMIT0_P_GPIOB18                                               25
#define     PAD_SMIT0_P_GPIOB3                                                24
#define     PAD_SMIT0_P_GPIOA25                                               23
#define     PAD_SMIT0_P_GPIOA15                                               22
#define     PAD_SMIT0_P_GPIOA0                                                21
#define     PAD_SMIT0_P_GPIOA24                                               20
#define     PAD_SMIT0_P_GPIOB1                                                19
#define     PAD_SMIT0_P_GPIOB17                                               18
#define     PAD_SMIT0_P_GPIOA23                                               17
#define     PAD_SMIT0_P_GPIOB4                                                16
#define     PAD_SMIT0_P_GPIOB8                                                15
#define     PAD_SMIT0_P_GPIOB16                                               14
#define     PAD_SMIT0_P_GPIOB21                                               13
#define     PAD_SMIT0_P_GPIOB5                                                12
#define     PAD_SMIT0_P_GPIOB13                                               11
#define     PAD_SMIT0_P_GPIOB0                                                10
#define     PAD_SMIT0_P_GPIOB12                                               9
#define     PAD_SMIT0_P_GPIOA2                                                8
#define     PAD_SMIT0_P_GPIOA3                                                7
#define     PAD_SMIT0_P_GPIOA14                                               6
#define     PAD_SMIT0_P_GPIOA18                                               5
#define     PAD_SMIT0_SPIBT_SS                                                4
#define     PAD_SMIT0_P_GPIOA19                                               3
#define     PAD_SMIT0_P_RESETB                                                2
#define     PAD_SMIT0_P_BSEL                                                  1
#define     PAD_SMIT0_P_TEST                                                  0

#define     PAD_SMIT1_P_GPIOA1                                                1
#define     PAD_SMIT1_P_GPIOA27                                               0

#define     PAD_DRV0_P_GPIOA18_e                                              30
#define     PAD_DRV0_P_GPIOA18_SHIFT                                          29
#define     PAD_DRV0_P_GPIOA18_MASK                                           (0x3<<29)
#define     PAD_DRV0_P_GPIOA17                                                28
#define     PAD_DRV0_P_GPIOA16_e                                              27
#define     PAD_DRV0_P_GPIOA16_SHIFT                                          26
#define     PAD_DRV0_P_GPIOA16_MASK                                           (0x3<<26)
#define     PAD_DRV0_P_GPIOA15                                                25
#define     PAD_DRV0_P_GPIOA14_e                                              24
#define     PAD_DRV0_P_GPIOA14_SHIFT                                          23
#define     PAD_DRV0_P_GPIOA14_MASK                                           (0x3<<23)
#define     PAD_DRV0_P_GPIOA13_e                                              22
#define     PAD_DRV0_P_GPIOA13_SHIFT                                          21
#define     PAD_DRV0_P_GPIOA13_MASK                                           (0x3<<21)
#define     PAD_DRV0_P_GPIOA12_e                                              20
#define     PAD_DRV0_P_GPIOA12_SHIFT                                          19
#define     PAD_DRV0_P_GPIOA12_MASK                                           (0x3<<19)
#define     PAD_DRV0_P_GPIOA11_e                                              18
#define     PAD_DRV0_P_GPIOA11_SHIFT                                          17
#define     PAD_DRV0_P_GPIOA11_MASK                                           (0x3<<17)
#define     PAD_DRV0_P_GPIOA10_e                                              16
#define     PAD_DRV0_P_GPIOA10_SHIFT                                          15
#define     PAD_DRV0_P_GPIOA10_MASK                                           (0x3<<15)
#define     PAD_DRV0_P_GPIOA9_e                                               14
#define     PAD_DRV0_P_GPIOA9_SHIFT                                           13
#define     PAD_DRV0_P_GPIOA9_MASK                                            (0x3<<13)
#define     PAD_DRV0_P_GPIOA8_e                                               12
#define     PAD_DRV0_P_GPIOA8_SHIFT                                           11
#define     PAD_DRV0_P_GPIOA8_MASK                                            (0x3<<11)
#define     PAD_DRV0_P_GPIOA7_e                                               10
#define     PAD_DRV0_P_GPIOA7_SHIFT                                           9
#define     PAD_DRV0_P_GPIOA7_MASK                                            (0x3<<9)
#define     PAD_DRV0_P_GPIOA6_e                                               8
#define     PAD_DRV0_P_GPIOA6_SHIFT                                           7
#define     PAD_DRV0_P_GPIOA6_MASK                                            (0x3<<7)
#define     PAD_DRV0_P_GPIOA5_e                                               6
#define     PAD_DRV0_P_GPIOA5_SHIFT                                           5
#define     PAD_DRV0_P_GPIOA5_MASK                                            (0x3<<5)
#define     PAD_DRV0_P_GPIOA4                                                 4
#define     PAD_DRV0_P_GPIOA3                                                 3
#define     PAD_DRV0_P_GPIOA2                                                 2
#define     PAD_DRV0_P_GPIOA1                                                 1
#define     PAD_DRV0_P_GPIOA0                                                 0

#define     PAD_DRV1_P_GPIOB2_e                                               30
#define     PAD_DRV1_P_GPIOB2_SHIFT                                           29
#define     PAD_DRV1_P_GPIOB2_MASK                                            (0x3<<29)
#define     PAD_DRV1_P_GPIOB1_e                                               28
#define     PAD_DRV1_P_GPIOB1_SHIFT                                           27
#define     PAD_DRV1_P_GPIOB1_MASK                                            (0x3<<27)
#define     PAD_DRV1_P_GPIOB0                                                 26
#define     PAD_DRV1_P_GPIOA31_e                                              25
#define     PAD_DRV1_P_GPIOA31_SHIFT                                          24
#define     PAD_DRV1_P_GPIOA31_MASK                                           (0x3<<24)
#define     PAD_DRV1_P_GPIOA30_e                                              23
#define     PAD_DRV1_P_GPIOA30_SHIFT                                          22
#define     PAD_DRV1_P_GPIOA30_MASK                                           (0x3<<22)
#define     PAD_DRV1_P_GPIOA29_e                                              21
#define     PAD_DRV1_P_GPIOA29_SHIFT                                          20
#define     PAD_DRV1_P_GPIOA29_MASK                                           (0x3<<20)
#define     PAD_DRV1_P_GPIOA28_e                                              19
#define     PAD_DRV1_P_GPIOA28_SHIFT                                          18
#define     PAD_DRV1_P_GPIOA28_MASK                                           (0x3<<18)
#define     PAD_DRV1_P_GPIOA27_e                                              17
#define     PAD_DRV1_P_GPIOA27_SHIFT                                          16
#define     PAD_DRV1_P_GPIOA27_MASK                                           (0x3<<16)
#define     PAD_DRV1_P_GPIOA26_e                                              15
#define     PAD_DRV1_P_GPIOA26_SHIFT                                          14
#define     PAD_DRV1_P_GPIOA26_MASK                                           (0x3<<14)
#define     PAD_DRV1_P_GPIOA25_e                                              13
#define     PAD_DRV1_P_GPIOA25_SHIFT                                          12
#define     PAD_DRV1_P_GPIOA25_MASK                                           (0x3<<12)
#define     PAD_DRV1_P_GPIOA24_e                                              11
#define     PAD_DRV1_P_GPIOA24_SHIFT                                          10
#define     PAD_DRV1_P_GPIOA24_MASK                                           (0x3<<10)
#define     PAD_DRV1_P_GPIOA23_e                                              9
#define     PAD_DRV1_P_GPIOA23_SHIFT                                          8
#define     PAD_DRV1_P_GPIOA23_MASK                                           (0x3<<8)
#define     PAD_DRV1_P_GPIOA22_e                                              7
#define     PAD_DRV1_P_GPIOA22_SHIFT                                          6
#define     PAD_DRV1_P_GPIOA22_MASK                                           (0x3<<6)
#define     PAD_DRV1_P_GPIOA21_e                                              5
#define     PAD_DRV1_P_GPIOA21_SHIFT                                          4
#define     PAD_DRV1_P_GPIOA21_MASK                                           (0x3<<4)
#define     PAD_DRV1_P_GPIOA20_e                                              3
#define     PAD_DRV1_P_GPIOA20_SHIFT                                          2
#define     PAD_DRV1_P_GPIOA20_MASK                                           (0x3<<2)
#define     PAD_DRV1_P_GPIOA19_e                                              1
#define     PAD_DRV1_P_GPIOA19_SHIFT                                          0
#define     PAD_DRV1_P_GPIOA19_MASK                                           (0x3<<0)

#define     PAD_DRV2_EJ_DINT                                                  31
#define     PAD_DRV2_P_GPIOB20                                                30
#define     PAD_DRV2_P_GPIOB19                                                29
#define     PAD_DRV2_P_GPIOB18                                                28
#define     PAD_DRV2_P_GPIOB17                                                27
#define     PAD_DRV2_P_GPIOB16                                                26
#define     PAD_DRV2_P_GPIOB15_e                                              25
#define     PAD_DRV2_P_GPIOB15_SHIFT                                          24
#define     PAD_DRV2_P_GPIOB15_MASK                                           (0x3<<24)
#define     PAD_DRV2_P_GPIOB14_e                                              23
#define     PAD_DRV2_P_GPIOB14_SHIFT                                          22
#define     PAD_DRV2_P_GPIOB14_MASK                                           (0x3<<22)
#define     PAD_DRV2_P_GPIOB13_e                                              21
#define     PAD_DRV2_P_GPIOB13_SHIFT                                          20
#define     PAD_DRV2_P_GPIOB13_MASK                                           (0x3<<20)
#define     PAD_DRV2_P_GPIOB12_e                                              19
#define     PAD_DRV2_P_GPIOB12_SHIFT                                          18
#define     PAD_DRV2_P_GPIOB12_MASK                                           (0x3<<18)
#define     PAD_DRV2_P_GPIOB11_e                                              17
#define     PAD_DRV2_P_GPIOB11_SHIFT                                          16
#define     PAD_DRV2_P_GPIOB11_MASK                                           (0x3<<16)
#define     PAD_DRV2_P_GPIOB10_e                                              15
#define     PAD_DRV2_P_GPIOB10_SHIFT                                          14
#define     PAD_DRV2_P_GPIOB10_MASK                                           (0x3<<14)
#define     PAD_DRV2_P_GPIOB9_e                                               13
#define     PAD_DRV2_P_GPIOB9_SHIFT                                           12
#define     PAD_DRV2_P_GPIOB9_MASK                                            (0x3<<12)
#define     PAD_DRV2_P_GPIOB8_e                                               11
#define     PAD_DRV2_P_GPIOB8_SHIFT                                           10
#define     PAD_DRV2_P_GPIOB8_MASK                                            (0x3<<10)
#define     PAD_DRV2_P_GPIOB7_e                                               9
#define     PAD_DRV2_P_GPIOB7_SHIFT                                           8
#define     PAD_DRV2_P_GPIOB7_MASK                                            (0x3<<8)
#define     PAD_DRV2_P_GPIOB6_e                                               7
#define     PAD_DRV2_P_GPIOB6_SHIFT                                           6
#define     PAD_DRV2_P_GPIOB6_MASK                                            (0x3<<6)
#define     PAD_DRV2_P_GPIOB5_e                                               5
#define     PAD_DRV2_P_GPIOB5_SHIFT                                           4
#define     PAD_DRV2_P_GPIOB5_MASK                                            (0x3<<4)
#define     PAD_DRV2_P_GPIOB4_e                                               3
#define     PAD_DRV2_P_GPIOB4_SHIFT                                           2
#define     PAD_DRV2_P_GPIOB4_MASK                                            (0x3<<2)
#define     PAD_DRV2_P_GPIOB3_e                                               1
#define     PAD_DRV2_P_GPIOB3_SHIFT                                           0
#define     PAD_DRV2_P_GPIOB3_MASK                                            (0x3<<0)

#define     PAD_DRV3_SPIBT_CLK_e                                              13
#define     PAD_DRV3_SPIBT_CLK_SHIFT                                          12
#define     PAD_DRV3_SPIBT_CLK_MASK                                           (0x3<<12)
#define     PAD_DRV3_SPIBT_MOSI_e                                             11
#define     PAD_DRV3_SPIBT_MOSI_SHIFT                                         10
#define     PAD_DRV3_SPIBT_MOSI_MASK                                          (0x3<<10)
#define     PAD_DRV3_SPIBT_MISO_e                                             9
#define     PAD_DRV3_SPIBT_MISO_SHIFT                                         8
#define     PAD_DRV3_SPIBT_MISO_MASK                                          (0x3<<8)
#define     PAD_DRV3_SPIBT_SS_e                                               7
#define     PAD_DRV3_SPIBT_SS_SHIFT                                           6
#define     PAD_DRV3_SPIBT_SS_MASK                                            (0x3<<6)
#define     PAD_DRV3_P_GPIOB23_e                                              5
#define     PAD_DRV3_P_GPIOB23_SHIFT                                          4
#define     PAD_DRV3_P_GPIOB23_MASK                                           (0x3<<4)
#define     PAD_DRV3_P_GPIOB22_e                                              3
#define     PAD_DRV3_P_GPIOB22_SHIFT                                          2
#define     PAD_DRV3_P_GPIOB22_MASK                                           (0x3<<2)
#define     PAD_DRV3_P_GPIOB21_e                                              1
#define     PAD_DRV3_P_GPIOB21_SHIFT                                          0
#define     PAD_DRV3_P_GPIOB21_MASK                                           (0x3<<0)

#define     PAD_DEBUG0_Module_selection_e                                     21
#define     PAD_DEBUG0_Module_selection_SHIFT                                 16
#define     PAD_DEBUG0_Module_selection_MASK                                  (0x3F<<16)
#define     PAD_DEBUG0_P_GPIOA30                                              12
#define     PAD_DEBUG0_P_GPIOA29                                              11
#define     PAD_DEBUG0_P_GPIOA28                                              10
#define     PAD_DEBUG0_P_GPIOA27                                              9
#define     PAD_DEBUG0_P_GPIOA26                                              8
#define     PAD_DEBUG0_P_GPIOA25                                              7
#define     PAD_DEBUG0_P_GPIOA24                                              6
#define     PAD_DEBUG0_P_GPIOA23                                              5
#define     PAD_DEBUG0_P_GPIOA16                                              4
#define     PAD_DEBUG0_P_GPIOA12                                              3
#define     PAD_DEBUG0_P_GPIOA11                                              2
#define     PAD_DEBUG0_P_GPIOA10                                              1
#define     PAD_DEBUG0_P_GPIOA9                                               0

#define     PAD_DEBUG1_P_GPIOB23                                              31
#define     PAD_DEBUG1_P_GPIOB22                                              30
#define     PAD_DEBUG1_P_GPIOB20                                              29
#define     PAD_DEBUG1_P_GPIOB19                                              28
#define     PAD_DEBUG1_P_GPIOB18                                              27
#define     PAD_DEBUG1_P_GPIOB17                                              26
#define     PAD_DEBUG1_P_GPIOB16                                              25
#define     PAD_DEBUG1_P_GPIOB15                                              24
#define     PAD_DEBUG1_P_GPIOB14                                              23
#define     PAD_DEBUG1_P_GPIOB13                                              22
#define     PAD_DEBUG1_P_GPIOB12                                              21
#define     PAD_DEBUG1_P_GPIOB7                                               20
#define     PAD_DEBUG1_P_GPIOB21                                              19
#define     PAD_DEBUG1_P_GPIOB11                                              18
#define     PAD_DEBUG1_P_GPIOB10                                              17
#define     PAD_DEBUG1_P_GPIOB9                                               16
#define     PAD_DEBUG1_P_GPIOB8                                               15
#define     PAD_DEBUG1_P_GPIOB2                                               14
#define     PAD_DEBUG1_P_GPIOB1                                               13
#define     PAD_DEBUG1_P_GPIOA30                                              12
#define     PAD_DEBUG1_P_GPIOA29                                              11
#define     PAD_DEBUG1_P_GPIOA28                                              10
#define     PAD_DEBUG1_P_GPIOA27                                              9
#define     PAD_DEBUG1_P_GPIOA26                                              8
#define     PAD_DEBUG1_P_GPIOA25                                              7
#define     PAD_DEBUG1_P_GPIOA24                                              6
#define     PAD_DEBUG1_P_GPIOA23                                              5
#define     PAD_DEBUG1_P_GPIOA16                                              4
#define     PAD_DEBUG1_P_GPIOA12                                              3
#define     PAD_DEBUG1_P_GPIOA11                                              2
#define     PAD_DEBUG1_P_GPIOA10                                              1
#define     PAD_DEBUG1_P_GPIOA9                                               0

//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
#ifdef PC
#undef     MemoryController_BASE
#undef     PMU_BASE
#undef     ChipVersion_BASE
#undef     RMU_digital_BASE                                                  
#undef     CMU_Analog_Register_BASE                                          
#undef     CMU_digital_BASE                                                  
#undef     RTC_BASE                                                          
#undef     InterruptController_BASE                                          
#undef     DMAController_BASE                                                
#undef     AUIP_BASE                                                         
#undef     MJPEG_BASE                                                        
#undef     NAND_Control_Register_BASE                                        
#undef     SDC_BASE                                                          
#undef     USB_Controller_Registers_BASE                                     
#undef     SPI_BASE                                                          
#undef     SPI_BOOT_BASE                                                     
#undef     I2C_BASE                                                          
#undef     UART_IR_BASE                                                      
#undef     LCDC_BASE                                                         
#undef     SEG_SREEN_BASE                                                    
#undef     GPIO_MFP_PWM_BASE                                                 
#undef     DAC_PA_I2S_Register_BASE    
#undef     ADC_Register_BASE                                          


#ifndef SIMBASE_API
#define SIMBASE_API __declspec( dllimport)
#endif
typedef unsigned char byte;

extern SIMBASE_API byte MemoryController_BASE[];
extern SIMBASE_API byte PMU_BASE[];
extern SIMBASE_API byte ChipVersion_BASE[];
extern SIMBASE_API byte RMU_digital_BASE[];
extern SIMBASE_API byte CMU_Analog_Register_BASE[];
extern SIMBASE_API byte CMU_digital_BASE[];
extern SIMBASE_API byte RTC_BASE[];
extern SIMBASE_API byte InterruptController_BASE[];
extern SIMBASE_API byte DMAController_BASE[];
extern SIMBASE_API byte AUIP_BASE[];
extern SIMBASE_API byte MJPEG_BASE[];
extern SIMBASE_API byte NAND_Control_Register_BASE[];
extern SIMBASE_API byte SDC_BASE[];
extern SIMBASE_API byte USB_Controller_Registers_BASE[];
extern SIMBASE_API byte SPI_BASE[];
extern SIMBASE_API byte SPI_BOOT_BASE[];
extern SIMBASE_API byte I2C_BASE[];
extern SIMBASE_API byte UART_IR_BASE[];
extern SIMBASE_API byte LCDC_BASE[];
extern SIMBASE_API byte SEG_SREEN_BASE[];
extern SIMBASE_API byte GPIO_MFP_PWM_BASE[];
extern SIMBASE_API byte DAC_PA_I2S_Register_BASE[];
extern SIMBASE_API byte ADC_Register_BASE[];
#endif


#endif





