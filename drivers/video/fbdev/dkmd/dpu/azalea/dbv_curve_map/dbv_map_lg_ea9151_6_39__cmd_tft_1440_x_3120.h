/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: dbv curve nolinear to linear map.
 * Create: 2019-07-29
 */

#ifndef DBV_MAP_LG_EA9151_6_39__CMD_TFT_1440_X_3120_H
#define DBV_MAP_LG_EA9151_6_39__CMD_TFT_1440_X_3120_H

#include "dbv_curve_nolinear_to_linear_map.h"

short dbv_curve_nolinear_to_linear_map_laya[DBV_MAP_TYPE_COUNT2][DBV_MAP_10BIT] = {
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 39, 55, 79, 94, 104, 119, 136, 142,
		146, 152, 156, 161, 162, 167, 171, 177, 182, 186,
		189, 192, 195, 201, 202, 206, 210, 214, 216, 219,
		225, 225, 229, 231, 234, 236, 240, 243, 245, 245,
		248, 250, 253, 257, 259, 262, 264, 267, 270, 273,
		276, 276, 278, 281, 284, 287, 290, 293, 293, 295,
		299, 301, 304, 306, 307, 309, 310, 311, 313, 316,
		319, 320, 320, 323, 325, 328, 330, 330, 332, 332,
		335, 337, 340, 340, 342, 344, 347, 349, 350, 350,
		351, 354, 356, 359, 360, 361, 361, 363, 366, 368,
		368, 371, 371, 372, 375, 377, 378, 380, 380, 382,
		385, 385, 387, 387, 390, 391, 394, 394, 394, 397,
		399, 401, 402, 402, 403, 406, 407, 408, 409, 411,
		411, 412, 413, 415, 416, 418, 418, 419, 421, 422,
		424, 424, 424, 425, 426, 428, 430, 431, 432, 432,
		434, 435, 437, 438, 440, 441, 441, 442, 444, 445,
		447, 449, 450, 450, 451, 452, 454, 456, 457, 458,
		458, 459, 460, 461, 463, 463, 464, 466, 467, 469,
		470, 470, 471, 474, 474, 476, 478, 479, 480, 482,
		483, 483, 485, 486, 486, 486, 488, 489, 490, 492,
		492, 493, 495, 497, 498, 499, 501, 502, 502, 504,
		505, 507, 508, 509, 509, 510, 511, 511, 512, 514,
		515, 517, 519, 520, 521, 521, 522, 524, 526, 527,
		529, 530, 531, 532, 532, 533, 534, 535, 536, 537,
		539, 540, 542, 543, 544, 544, 544, 545, 546, 547,
		548, 549, 550, 551, 551, 551, 551, 552, 553, 554,
		555, 555, 556, 557, 558, 559, 560, 561, 562, 563,
		564, 565, 565, 566, 566, 567, 567, 568, 569, 569,
		571, 571, 572, 573, 574, 575, 576, 577, 577, 578,
		579, 579, 580, 581, 582, 583, 583, 583, 583, 584,
		585, 586, 587, 588, 589, 590, 591, 592, 593, 594,
		595, 596, 597, 598, 598, 599, 599, 599, 600, 601,
		601, 603, 603, 603, 604, 606, 606, 607, 608, 609,
		610, 611, 612, 613, 614, 615, 615, 615, 615, 615,
		616, 617, 619, 620, 621, 622, 623, 624, 625, 626,
		627, 627, 628, 629, 630, 630, 631, 631, 631, 631,
		632, 633, 633, 635, 635, 636, 638, 639, 640, 641,
		642, 642, 643, 644, 645, 646, 646, 646, 646, 647,
		647, 648, 649, 650, 650, 652, 653, 654, 655, 656,
		657, 658, 659, 661, 662, 662, 662, 662, 662, 663,
		664, 664, 665, 666, 667, 667, 669, 670, 671, 672,
		672, 674, 675, 676, 677, 678, 678, 678, 678, 678,
		679, 680, 681, 681, 682, 684, 685, 686, 686, 686,
		687, 689, 690, 691, 692, 693, 694, 694, 694, 694,
		694, 695, 696, 696, 697, 699, 699, 701, 702, 703,
		704, 705, 706, 707, 708, 709, 710, 710, 710, 710,
		710, 710, 711, 712, 713, 714, 714, 716, 717, 719,
		720, 720, 722, 722, 725, 725, 725, 725, 726, 726,
		726, 726, 726, 727, 730, 730, 732, 733, 735, 735,
		735, 735, 735, 735, 735, 735, 735, 735, 735, 736,
		736, 737, 737, 738, 738, 738, 739, 739, 739, 740,
		740, 741, 741, 742, 742, 742, 743, 743, 744, 744,
		744, 745, 745, 746, 746, 746, 747, 747, 747, 747,
		748, 748, 749, 749, 750, 750, 751, 751, 751, 752,
		752, 752, 753, 753, 754, 754, 754, 754, 755, 756,
		756, 756, 757, 757, 758, 758, 758, 759, 759, 759,
		759, 760, 760, 761, 762, 762, 762, 762, 763, 763,
		764, 764, 764, 764, 765, 765, 765, 766, 766, 766,
		767, 768, 768, 769, 769, 769, 769, 770, 770, 771,
		772, 772, 773, 774, 775, 776, 776, 777, 777, 779,
		779, 779, 780, 781, 781, 781, 782, 783, 783, 784,
		785, 786, 786, 787, 788, 788, 789, 789, 790, 791,
		792, 792, 793, 793, 794, 795, 795, 796, 797, 797,
		798, 798, 799, 799, 799, 804, 805, 805, 806, 806,
		807, 808, 809, 810, 810, 811, 812, 812, 813, 813,
		814, 815, 816, 816, 817, 817, 818, 818, 819, 820,
		820, 821, 821, 822, 822, 823, 824, 824, 824, 826,
		827, 827, 828, 828, 829, 830, 830, 831, 832, 833,
		833, 833, 834, 835, 835, 836, 836, 837, 838, 838,
		838, 839, 840, 840, 841, 841, 842, 843, 844, 845,
		845, 845, 846, 847, 848, 848, 848, 850, 850, 851,
		851, 852, 852, 853, 853, 853, 854, 855, 856, 856,
		857, 857, 858, 858, 859, 859, 861, 861, 862, 862,
		863, 864, 864, 864, 865, 866, 867, 867, 868, 868,
		869, 869, 870, 870, 871, 871, 872, 873, 873, 874,
		874, 875, 875, 876, 876, 876, 878, 879, 879, 880,
		881, 881, 882, 882, 883, 884, 884, 885, 885, 886,
		886, 887, 887, 887, 888, 889, 890, 890, 891, 891,
		892, 892, 893, 893, 894, 894, 895, 896, 896, 897,
		898, 899, 899, 899, 901, 901, 902, 902, 902, 903,
		903, 904, 904, 905, 905, 906, 907, 907, 908, 908,
		909, 909, 910, 910, 911, 912, 912, 913, 914, 915,
		915, 915, 916, 917, 918, 918, 919, 919, 919, 920,
		920, 921, 921, 921, 922, 922, 923, 924, 924, 925,
		926, 926, 926, 927, 928, 928, 928, 929, 930, 930,
		931, 932, 932, 933, 933, 934, 935, 935, 936, 936,
		937, 937, 938, 938, 938, 939, 940, 940, 941, 942,
		942, 943, 943, 944, 944, 944, 946, 946, 947, 947,
		948, 949, 949, 950, 950, 950, 951, 952, 953, 953,
		954, 954, 955, 955, 955, 956, 956, 957, 958, 958,
		959, 959, 960, 960, 961, 961, 962, 963, 963, 964,
		964, 965, 966, 966, 966, 967, 967, 968, 969, 969,
		970, 970, 970, 971, 972, 972, 972, 973, 973, 974,
		975, 976, 976, 976, 977, 977, 978, 978, 979, 980,
		980, 981, 981, 982, 983, 983, 984, 984, 984, 985,
		986, 986, 987, 987, 988, 988, 989, 989, 989, 990,
		990, 991, 992, 993, 994, 995, 996, 997, 998, 999,
		1000, 1000, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
		1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019,
		1020, 1021, 1022, 1023,
	},
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 38, 51, 52, 60, 64, 73, 74, 82,
		84, 90, 94, 94, 99, 105, 105, 109, 114, 114,
		118, 123, 123, 129, 133, 133, 138, 138, 143, 147,
		147, 152, 152, 155, 159, 159, 163, 166, 166, 170,
		173, 177, 181, 181, 184, 188, 190, 190, 194, 198,
		201, 201, 205, 208, 208, 212, 216, 218, 222, 225,
		225, 229, 233, 234, 235, 235, 237, 238, 241, 241,
		243, 244, 246, 246, 249, 250, 252, 252, 254, 256,
		258, 258, 260, 262, 263, 263, 266, 268, 269, 269,
		271, 273, 275, 275, 277, 279, 281, 282, 282, 285,
		287, 288, 288, 290, 292, 294, 294, 296, 298, 300,
		301, 301, 301, 301, 304, 306, 307, 309, 309, 311,
		313, 315, 315, 317, 319, 321, 322, 322, 324, 324,
		325, 326, 326, 327, 328, 330, 330, 330, 332, 333,
		333, 334, 335, 336, 337, 337, 339, 339, 340, 341,
		342, 344, 344, 345, 346, 347, 347, 348, 349, 350,
		351, 353, 355, 355, 355, 357, 358, 359, 360, 360,
		361, 362, 363, 364, 364, 365, 366, 367, 369, 370,
		371, 372, 372, 373, 374, 375, 376, 376, 377, 378,
		380, 380, 382, 382, 382, 384, 385, 386, 386, 387,
		388, 389, 390, 391, 391, 392, 393, 394, 396, 396,
		398, 398, 399, 400, 401, 402, 403, 403, 404, 405,
		406, 407, 408, 410, 411, 412, 413, 413, 414, 415,
		416, 417, 418, 419, 419, 420, 421, 423, 423, 425,
		426, 426, 427, 428, 429, 430, 431, 432, 432, 433,
		434, 434, 434, 436, 437, 438, 439, 441, 441, 442,
		443, 444, 446, 447, 448, 449, 449, 450, 452, 453,
		454, 454, 454, 454, 454, 455, 456, 458, 459, 460,
		462, 462, 462, 464, 465, 466, 468, 469, 470, 471,
		471, 472, 474, 475, 476, 478, 478, 480, 481, 482,
		482, 485, 485, 486, 487, 488, 490, 491, 492, 494,
		494, 494, 494, 494, 494, 495, 496, 497, 499, 500,
		501, 502, 503, 504, 504, 507, 507, 508, 510, 510,
		512, 513, 514, 515, 517, 518, 519, 520, 521, 521,
		523, 524, 525, 526, 528, 529, 530, 531, 533, 534,
		534, 534, 534, 534, 534, 535, 536, 537, 537, 539,
		540, 541, 542, 543, 545, 546, 547, 549, 550, 551,
		552, 553, 555, 556, 556, 557, 558, 559, 561, 562,
		563, 565, 565, 567, 568, 569, 569, 569, 569, 569,
		571, 572, 573, 574, 574, 574, 575, 577, 578, 579,
		580, 581, 583, 584, 585, 587, 587, 589, 590, 591,
		593, 594, 594, 596, 597, 597, 599, 599, 599, 599,
		599, 600, 601, 602, 603, 605, 606, 607, 608, 610,
		611, 611, 612, 613, 615, 615, 616, 618, 619, 621,
		622, 623, 624, 625, 627, 627, 628, 629, 629, 629,
		629, 629, 630, 631, 632, 633, 634, 635, 638, 639,
		640, 641, 642, 644, 644, 645, 646, 647, 648, 650,
		651, 652, 654, 654, 655, 656, 658, 659, 659, 659,
		659, 659, 659, 661, 661, 662, 663, 664, 666, 668,
		670, 670, 672, 674, 674, 674, 674, 674, 674, 674,
		674, 674, 674, 674, 675, 675, 675, 676, 676, 677,
		677, 677, 677, 678, 679, 679, 679, 680, 680, 681,
		681, 681, 682, 682, 683, 683, 683, 684, 684, 685,
		685, 685, 686, 686, 687, 687, 687, 688, 689, 689,
		689, 690, 690, 693, 693, 694, 694, 695, 696, 696,
		696, 696, 697, 698, 698, 698, 698, 699, 699, 700,
		700, 700, 701, 702, 702, 702, 703, 704, 704, 704,
		704, 705, 706, 706, 706, 706, 707, 708, 708, 708,
		709, 709, 710, 710, 710, 711, 711, 712, 712, 712,
		713, 713, 714, 714, 715, 715, 715, 716, 716, 717,
		717, 718, 719, 719, 719, 720, 721, 722, 727, 727,
		729, 729, 730, 731, 731, 732, 733, 733, 734, 735,
		735, 736, 736, 737, 738, 738, 740, 740, 741, 742,
		742, 743, 744, 744, 745, 746, 747, 747, 748, 749,
		750, 750, 751, 752, 752, 752, 753, 754, 760, 761,
		761, 762, 763, 763, 765, 765, 765, 766, 767, 768,
		769, 769, 770, 771, 771, 771, 772, 773, 773, 774,
		776, 776, 776, 777, 778, 778, 779, 780, 780, 781,
		782, 782, 783, 784, 784, 785, 786, 786, 791, 792,
		792, 794, 794, 795, 796, 797, 797, 798, 799, 799,
		799, 801, 801, 802, 803, 803, 803, 804, 805, 805,
		806, 807, 807, 809, 809, 810, 811, 811, 812, 813,
		813, 814, 815, 816, 816, 816, 817, 818, 824, 824,
		825, 825, 826, 826, 827, 828, 828, 829, 830, 831,
		831, 832, 832, 833, 834, 835, 835, 836, 837, 837,
		838, 839, 839, 839, 840, 841, 841, 842, 843, 843,
		843, 844, 845, 845, 846, 847, 848, 848, 849, 849,
		849, 855, 857, 857, 858, 859, 859, 860, 861, 861,
		862, 862, 863, 864, 864, 864, 865, 865, 866, 866,
		867, 868, 868, 869, 870, 870, 870, 871, 872, 873,
		873, 874, 874, 877, 878, 878, 879, 879, 880, 880,
		881, 882, 882, 889, 889, 889, 890, 891, 891, 892,
		893, 894, 895, 895, 895, 896, 897, 897, 897, 899,
		899, 899, 900, 901, 901, 901, 902, 903, 903, 904,
		904, 905, 905, 906, 907, 907, 907, 909, 909, 910,
		911, 911, 912, 912, 913, 913, 914, 920, 920, 920,
		921, 922, 922, 923, 923, 924, 925, 925, 926, 927,
		927, 928, 928, 929, 930, 930, 931, 932, 932, 933,
		933, 934, 934, 935, 935, 936, 936, 936, 938, 938,
		938, 938, 939, 940, 940, 941, 942, 943, 943, 944,
		945, 945, 946, 952, 953, 953, 954, 954, 956, 957,
		957, 957, 958, 959, 959, 960, 960, 961, 961, 963,
		963, 963, 964, 965, 966, 966, 967, 968, 968, 968,
		969, 970, 970, 970, 971, 971, 972, 972, 973, 974,
		974, 974, 975, 976, 976, 976, 977, 978, 978, 978,
		986, 986, 987, 987, 988, 988, 989, 989, 990, 990,
		991, 991, 992, 992, 994, 995, 996, 997, 998, 999,
		1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
		1010, 1011, 1011, 1013, 1014, 1015, 1016, 1017, 1018, 1019,
		1020, 1021, 1022, 1023,
	},
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 38, 46, 54, 59, 66, 72, 79, 87,
		93, 98, 103, 109, 116, 119, 122, 123, 125, 127,
		129, 130, 131, 133, 134, 136, 138, 140, 141, 143,
		145, 146, 147, 149, 150, 151, 155, 158, 162, 167,
		170, 174, 178, 182, 185, 189, 192, 196, 199, 202,
		206, 209, 213, 215, 219, 222, 225, 228, 231, 235,
		236, 237, 238, 240, 241, 243, 243, 246, 247, 249,
		250, 252, 252, 254, 255, 256, 258, 259, 260, 262,
		263, 265, 266, 268, 269, 271, 273, 274, 275, 277,
		278, 280, 281, 282, 284, 285, 287, 288, 289, 290,
		292, 294, 295, 297, 298, 299, 301, 303, 304, 306,
		308, 309, 310, 311, 313, 314, 315, 317, 319, 320,
		322, 323, 324, 324, 326, 327, 327, 329, 330, 331,
		332, 333, 334, 335, 336, 337, 338, 340, 341, 341,
		343, 344, 344, 346, 347, 348, 349, 350, 350, 352,
		353, 354, 355, 356, 356, 358, 358, 359, 361, 361,
		362, 364, 364, 365, 366, 367, 368, 369, 370, 370,
		372, 372, 373, 374, 375, 376, 377, 378, 378, 379,
		380, 381, 382, 383, 384, 384, 385, 386, 387, 388,
		389, 390, 390, 391, 392, 393, 394, 395, 396, 396,
		398, 398, 399, 400, 401, 402, 402, 403, 404, 405,
		405, 406, 407, 408, 408, 410, 410, 411, 412, 413,
		413, 414, 415, 416, 417, 417, 418, 419, 420, 421,
		421, 422, 422, 424, 424, 425, 426, 427, 427, 428,
		429, 430, 430, 431, 432, 433, 433, 434, 434, 434,
		435, 436, 437, 437, 439, 440, 441, 442, 443, 444,
		445, 446, 447, 447, 449, 450, 451, 452, 453, 453,
		455, 456, 457, 458, 459, 459, 460, 461, 462, 464,
		465, 465, 466, 467, 468, 469, 469, 472, 472, 473,
		474, 474, 475, 477, 478, 479, 480, 481, 482, 483,
		484, 485, 485, 487, 488, 489, 490, 491, 491, 493,
		494, 495, 496, 496, 497, 499, 500, 501, 502, 502,
		504, 505, 506, 507, 507, 508, 510, 511, 512, 513,
		513, 514, 516, 517, 518, 518, 520, 520, 522, 523,
		524, 524, 526, 527, 528, 529, 529, 530, 532, 533,
		534, 534, 535, 535, 537, 538, 539, 540, 540, 542,
		544, 545, 545, 546, 548, 549, 550, 551, 551, 552,
		554, 555, 556, 556, 557, 558, 560, 561, 562, 562,
		564, 565, 566, 567, 567, 568, 570, 571, 572, 573,
		573, 574, 576, 577, 578, 578, 580, 581, 583, 584,
		584, 584, 586, 587, 588, 589, 589, 590, 592, 593,
		594, 594, 595, 596, 599, 600, 600, 601, 602, 604,
		605, 605, 605, 606, 607, 609, 610, 611, 611, 614,
		614, 615, 616, 616, 618, 620, 621, 622, 622, 622,
		623, 626, 626, 627, 627, 628, 629, 632, 633, 633,
		633, 635, 637, 638, 638, 639, 639, 641, 642, 644,
		644, 645, 645, 647, 647, 649, 650, 651, 653, 654,
		654, 655, 656, 657, 659, 660, 660, 661, 662, 663,
		664, 665, 667, 669, 670, 671, 671, 673, 673, 674,
		674, 674, 674, 674, 674, 674, 674, 674, 674, 675,
		676, 676, 677, 678, 679, 680, 680, 681, 681, 682,
		682, 683, 683, 683, 683, 683, 683, 683, 684, 685,
		686, 687, 688, 688, 689, 690, 691, 691, 692, 693,
		693, 694, 695, 695, 696, 697, 697, 698, 699, 699,
		700, 700, 700, 701, 702, 703, 703, 703, 703, 703,
		703, 703, 704, 705, 706, 706, 707, 708, 708, 709,
		710, 710, 711, 712, 712, 713, 714, 714, 715, 716,
		717, 717, 718, 718, 718, 719, 720, 720, 721, 721,
		722, 723, 723, 723, 723, 723, 723, 723, 724, 725,
		725, 726, 727, 727, 728, 729, 729, 730, 730, 731,
		731, 733, 733, 734, 735, 735, 736, 736, 737, 737,
		738, 739, 739, 740, 741, 742, 742, 743, 743, 743,
		743, 743, 743, 743, 743, 744, 745, 745, 746, 746,
		747, 748, 748, 749, 750, 750, 751, 752, 752, 753,
		753, 753, 754, 754, 755, 756, 757, 757, 758, 761,
		762, 763, 763, 763, 763, 763, 763, 763, 763, 764,
		765, 766, 766, 768, 769, 770, 771, 772, 773, 774,
		775, 776, 778, 780, 780, 783, 783, 783, 783, 783,
		783, 783, 783, 784, 785, 786, 787, 788, 789, 789,
		790, 791, 792, 794, 796, 796, 798, 798, 799, 801,
		802, 802, 803, 803, 803, 803, 803, 803, 803, 804,
		806, 807, 807, 808, 809, 809, 811, 812, 813, 815,
		815, 817, 818, 819, 820, 821, 822, 823, 823, 823,
		823, 823, 823, 823, 824, 825, 825, 825, 827, 827,
		829, 831, 832, 833, 834, 835, 836, 837, 838, 838,
		840, 841, 842, 842, 843, 843, 843, 843, 843, 843,
		843, 843, 844, 844, 846, 847, 850, 852, 853, 854,
		855, 856, 857, 857, 857, 858, 859, 861, 862, 863,
		863, 863, 863, 863, 863, 863, 863, 863, 864, 865,
		867, 868, 870, 871, 872, 873, 873, 873, 875, 876,
		876, 878, 878, 879, 880, 882, 883, 883, 883, 883,
		883, 883, 883, 883, 883, 885, 886, 888, 888, 889,
		889, 890, 891, 891, 894, 894, 895, 896, 897, 898,
		899, 900, 902, 903, 903, 903, 903, 903, 903, 903,
		904, 904, 905, 906, 906, 907, 908, 909, 910, 911,
		912, 913, 914, 915, 916, 917, 918, 920, 921, 921,
		921, 922, 923, 923, 923, 923, 923, 923, 923, 923,
		923, 925, 927, 927, 929, 929, 930, 931, 932, 933,
		934, 936, 937, 937, 938, 939, 940, 941, 942, 943,
		943, 943, 943, 943, 943, 943, 943, 943, 944, 944,
		946, 947, 948, 949, 949, 950, 951, 952, 953, 953,
		953, 954, 959, 960, 961, 961, 962, 963, 963, 963,
		963, 963, 963, 963, 963, 963, 964, 965, 966, 966,
		968, 969, 969, 969, 970, 971, 972, 973, 974, 978,
		979, 979, 980, 982, 983, 983, 983, 983, 983, 983,
		983, 983, 983, 983, 984, 985, 985, 985, 986, 986,
		989, 990, 991, 992, 994, 995, 996, 997, 998, 999,
		1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009,
		1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019,
		1020, 1021, 1022, 1023,
	}
};

#endif
