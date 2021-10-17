#include "yack/system/setup.h"
const uint8_t yack_xbitrev_I4[1]={
3
};

const uint8_t yack_xbitrev_J4[1]={
5
};

const uint8_t yack_xbitrev_I8[2]={
3, 7
};

const uint8_t yack_xbitrev_J8[2]={
9, 13
};

const uint8_t yack_xbitrev_I16[6]={
3, 5, 7, 11, 15, 23
};

const uint8_t yack_xbitrev_J16[6]={
17, 9, 25, 21, 29, 27
};

const uint8_t yack_xbitrev_I32[12]={
3, 5, 7, 11, 13, 15, 19, 23, 27, 31, 39, 47
};

const uint8_t yack_xbitrev_J32[12]={
33, 17, 49, 41, 25, 57, 37, 53, 45, 61, 51, 59
};

const uint8_t yack_xbitrev_I64[28]={
3, 5, 7, 9, 11, 13, 15, 19, 21, 23, 27, 29, 31, 35, 39, 43, 45, 47, 51, 55, 59, 63, 71, 75, 79, 87, 95, 111
};

const uint8_t yack_xbitrev_J64[28]={
65, 33, 97, 17, 81, 49, 113, 73, 41, 105, 89, 57, 121, 69, 101, 85, 53, 117, 77, 109, 93, 125, 99, 83, 115, 107, 123, 119
};

const uint8_t yack_xbitrev_I128[56]={
3, 5, 7, 9, 11, 13, 15, 19, 21, 23, 25, 27, 29, 31, 35, 37, 39, 43, 45, 47, 51, 53, 55, 59, 61, 63, 67, 71, 75, 77, 79, 83, 87, 91, 93, 95, 99, 103, 107, 111, 115, 119, 123, 127, 135, 139, 143, 151, 155, 159, 167, 175, 183, 191, 207, 223
};

const uint8_t yack_xbitrev_J128[56]={
129, 65, 193, 33, 161, 97, 225, 145, 81, 209, 49, 177, 113, 241, 137, 73, 201, 169, 105, 233, 153, 89, 217, 185, 121, 249, 133, 197, 165, 101, 229, 149, 213, 181, 117, 245, 141, 205, 173, 237, 157, 221, 189, 253, 195, 163, 227, 211, 179, 243, 203, 235, 219, 251, 231, 247
};

const uint16_t yack_xbitrev_I256[120]={
3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 35, 37, 39, 41, 43, 45, 47, 51, 53, 55, 57, 59, 61, 63, 67, 69, 71, 75, 77, 79, 83, 85, 87, 89, 91, 93, 95, 99, 101, 103, 107, 109, 111, 115, 117, 119, 123, 125, 127, 131, 135, 139, 141, 143, 147, 149, 151, 155, 157, 159, 163, 167, 171, 173, 175, 179, 183, 187, 189, 191, 195, 199, 203, 207, 211, 215, 219, 221, 223, 227, 231, 235, 239, 243, 247, 251, 255, 263, 267, 271, 275, 279, 283, 287, 295, 299, 303, 311, 315, 319, 327, 335, 343, 347, 351, 359, 367, 375, 383, 399, 407, 415, 431, 447, 479
};

const uint16_t yack_xbitrev_J256[120]={
257, 129, 385, 65, 321, 193, 449, 33, 289, 161, 417, 97, 353, 225, 481, 273, 145, 401, 81, 337, 209, 465, 305, 177, 433, 113, 369, 241, 497, 265, 137, 393, 329, 201, 457, 297, 169, 425, 105, 361, 233, 489, 281, 153, 409, 345, 217, 473, 313, 185, 441, 377, 249, 505, 261, 389, 325, 197, 453, 293, 165, 421, 357, 229, 485, 277, 405, 341, 213, 469, 309, 437, 373, 245, 501, 269, 397, 333, 461, 301, 429, 365, 237, 493, 285, 413, 349, 477, 317, 445, 381, 509, 387, 323, 451, 291, 419, 355, 483, 403, 339, 467, 435, 371, 499, 395, 459, 427, 363, 491, 411, 475, 443, 507, 455, 423, 487, 471, 503, 495
};

const uint16_t yack_xbitrev_I512[240]={
3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 67, 69, 71, 73, 75, 77, 79, 83, 85, 87, 89, 91, 93, 95, 99, 101, 103, 105, 107, 109, 111, 115, 117, 119, 121, 123, 125, 127, 131, 133, 135, 139, 141, 143, 147, 149, 151, 153, 155, 157, 159, 163, 165, 167, 171, 173, 175, 179, 181, 183, 185, 187, 189, 191, 195, 197, 199, 203, 205, 207, 211, 213, 215, 219, 221, 223, 227, 229, 231, 235, 237, 239, 243, 245, 247, 251, 253, 255, 259, 263, 267, 269, 271, 275, 277, 279, 283, 285, 287, 291, 295, 299, 301, 303, 307, 309, 311, 315, 317, 319, 323, 327, 331, 333, 335, 339, 343, 347, 349, 351, 355, 359, 363, 365, 367, 371, 375, 379, 381, 383, 387, 391, 395, 399, 403, 407, 411, 413, 415, 419, 423, 427, 431, 435, 439, 443, 445, 447, 451, 455, 459, 463, 467, 471, 475, 479, 483, 487, 491, 495, 499, 503, 507, 511, 519, 523, 527, 531, 535, 539, 543, 551, 555, 559, 563, 567, 571, 575, 583, 587, 591, 599, 603, 607, 615, 619, 623, 631, 635, 639, 647, 655, 663, 667, 671, 679, 687, 695, 699, 703, 711, 719, 727, 735, 743, 751, 759, 767, 783, 791, 799, 815, 823, 831, 847, 863, 879, 895, 927, 959
};

const uint16_t yack_xbitrev_J512[240]={
513, 257, 769, 129, 641, 385, 897, 65, 577, 321, 833, 193, 705, 449, 961, 545, 289, 801, 161, 673, 417, 929, 97, 609, 353, 865, 225, 737, 481, 993, 529, 273, 785, 145, 657, 401, 913, 593, 337, 849, 209, 721, 465, 977, 561, 305, 817, 177, 689, 433, 945, 625, 369, 881, 241, 753, 497, 1009, 521, 265, 777, 649, 393, 905, 585, 329, 841, 201, 713, 457, 969, 553, 297, 809, 681, 425, 937, 617, 361, 873, 233, 745, 489, 1001, 537, 281, 793, 665, 409, 921, 601, 345, 857, 729, 473, 985, 569, 313, 825, 697, 441, 953, 633, 377, 889, 761, 505, 1017, 517, 773, 645, 389, 901, 581, 325, 837, 709, 453, 965, 549, 805, 677, 421, 933, 613, 357, 869, 741, 485, 997, 533, 789, 661, 405, 917, 597, 853, 725, 469, 981, 565, 821, 693, 437, 949, 629, 885, 757, 501, 1013, 525, 781, 653, 909, 589, 845, 717, 461, 973, 557, 813, 685, 941, 621, 877, 749, 493, 1005, 541, 797, 669, 925, 605, 861, 733, 989, 573, 829, 701, 957, 637, 893, 765, 1021, 771, 643, 899, 579, 835, 707, 963, 803, 675, 931, 611, 867, 739, 995, 787, 659, 915, 851, 723, 979, 819, 691, 947, 883, 755, 1011, 779, 907, 843, 715, 971, 811, 939, 875, 747, 1003, 795, 923, 859, 987, 827, 955, 891, 1019, 903, 839, 967, 935, 871, 999, 919, 983, 951, 1015, 975, 1007
};

const uint16_t yack_xbitrev_I1024[496]={
3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125, 127, 131, 133, 135, 137, 139, 141, 143, 147, 149, 151, 153, 155, 157, 159, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189, 191, 195, 197, 199, 201, 203, 205, 207, 211, 213, 215, 217, 219, 221, 223, 227, 229, 231, 233, 235, 237, 239, 243, 245, 247, 249, 251, 253, 255, 259, 261, 263, 267, 269, 271, 275, 277, 279, 281, 283, 285, 287, 291, 293, 295, 297, 299, 301, 303, 307, 309, 311, 313, 315, 317, 319, 323, 325, 327, 331, 333, 335, 339, 341, 343, 345, 347, 349, 351, 355, 357, 359, 363, 365, 367, 371, 373, 375, 377, 379, 381, 383, 387, 389, 391, 395, 397, 399, 403, 405, 407, 411, 413, 415, 419, 421, 423, 427, 429, 431, 435, 437, 439, 441, 443, 445, 447, 451, 453, 455, 459, 461, 463, 467, 469, 471, 475, 477, 479, 483, 485, 487, 491, 493, 495, 499, 501, 503, 507, 509, 511, 515, 519, 523, 525, 527, 531, 533, 535, 539, 541, 543, 547, 549, 551, 555, 557, 559, 563, 565, 567, 571, 573, 575, 579, 583, 587, 589, 591, 595, 597, 599, 603, 605, 607, 611, 615, 619, 621, 623, 627, 629, 631, 635, 637, 639, 643, 647, 651, 653, 655, 659, 663, 667, 669, 671, 675, 679, 683, 685, 687, 691, 693, 695, 699, 701, 703, 707, 711, 715, 717, 719, 723, 727, 731, 733, 735, 739, 743, 747, 749, 751, 755, 759, 763, 765, 767, 771, 775, 779, 783, 787, 791, 795, 797, 799, 803, 807, 811, 813, 815, 819, 823, 827, 829, 831, 835, 839, 843, 847, 851, 855, 859, 861, 863, 867, 871, 875, 879, 883, 887, 891, 893, 895, 899, 903, 907, 911, 915, 919, 923, 927, 931, 935, 939, 943, 947, 951, 955, 957, 959, 963, 967, 971, 975, 979, 983, 987, 991, 995, 999, 1003, 1007, 1011, 1015, 1019, 1023, 1031, 1035, 1039, 1043, 1047, 1051, 1055, 1059, 1063, 1067, 1071, 1075, 1079, 1083, 1087, 1095, 1099, 1103, 1107, 1111, 1115, 1119, 1127, 1131, 1135, 1139, 1143, 1147, 1151, 1159, 1163, 1167, 1175, 1179, 1183, 1191, 1195, 1199, 1203, 1207, 1211, 1215, 1223, 1227, 1231, 1239, 1243, 1247, 1255, 1259, 1263, 1271, 1275, 1279, 1287, 1295, 1303, 1307, 1311, 1319, 1323, 1327, 1335, 1339, 1343, 1351, 1359, 1367, 1371, 1375, 1383, 1391, 1399, 1403, 1407, 1415, 1423, 1431, 1439, 1447, 1455, 1463, 1467, 1471, 1479, 1487, 1495, 1503, 1511, 1519, 1527, 1535, 1551, 1559, 1567, 1575, 1583, 1591, 1599, 1615, 1623, 1631, 1647, 1655, 1663, 1679, 1695, 1711, 1719, 1727, 1743, 1759, 1775, 1791, 1823, 1839, 1855, 1887, 1919, 1983
};

const uint16_t yack_xbitrev_J1024[496]={
1025, 513, 1537, 257, 1281, 769, 1793, 129, 1153, 641, 1665, 385, 1409, 897, 1921, 65, 1089, 577, 1601, 321, 1345, 833, 1857, 193, 1217, 705, 1729, 449, 1473, 961, 1985, 1057, 545, 1569, 289, 1313, 801, 1825, 161, 1185, 673, 1697, 417, 1441, 929, 1953, 1121, 609, 1633, 353, 1377, 865, 1889, 225, 1249, 737, 1761, 481, 1505, 993, 2017, 1041, 529, 1553, 273, 1297, 785, 1809, 1169, 657, 1681, 401, 1425, 913, 1937, 1105, 593, 1617, 337, 1361, 849, 1873, 209, 1233, 721, 1745, 465, 1489, 977, 2001, 1073, 561, 1585, 305, 1329, 817, 1841, 1201, 689, 1713, 433, 1457, 945, 1969, 1137, 625, 1649, 369, 1393, 881, 1905, 1265, 753, 1777, 497, 1521, 1009, 2033, 1033, 521, 1545, 1289, 777, 1801, 1161, 649, 1673, 393, 1417, 905, 1929, 1097, 585, 1609, 329, 1353, 841, 1865, 1225, 713, 1737, 457, 1481, 969, 1993, 1065, 553, 1577, 1321, 809, 1833, 1193, 681, 1705, 425, 1449, 937, 1961, 1129, 617, 1641, 1385, 873, 1897, 1257, 745, 1769, 489, 1513, 1001, 2025, 1049, 537, 1561, 1305, 793, 1817, 1177, 665, 1689, 1433, 921, 1945, 1113, 601, 1625, 1369, 857, 1881, 1241, 729, 1753, 473, 1497, 985, 2009, 1081, 569, 1593, 1337, 825, 1849, 1209, 697, 1721, 1465, 953, 1977, 1145, 633, 1657, 1401, 889, 1913, 1273, 761, 1785, 1529, 1017, 2041, 1029, 1541, 1285, 773, 1797, 1157, 645, 1669, 1413, 901, 1925, 1093, 581, 1605, 1349, 837, 1861, 1221, 709, 1733, 1477, 965, 1989, 1061, 1573, 1317, 805, 1829, 1189, 677, 1701, 1445, 933, 1957, 1125, 1637, 1381, 869, 1893, 1253, 741, 1765, 1509, 997, 2021, 1045, 1557, 1301, 789, 1813, 1173, 1685, 1429, 917, 1941, 1109, 1621, 1365, 853, 1877, 1237, 725, 1749, 1493, 981, 2005, 1077, 1589, 1333, 821, 1845, 1205, 1717, 1461, 949, 1973, 1141, 1653, 1397, 885, 1909, 1269, 1781, 1525, 1013, 2037, 1037, 1549, 1293, 1805, 1165, 1677, 1421, 909, 1933, 1101, 1613, 1357, 845, 1869, 1229, 1741, 1485, 973, 1997, 1069, 1581, 1325, 1837, 1197, 1709, 1453, 941, 1965, 1133, 1645, 1389, 1901, 1261, 1773, 1517, 1005, 2029, 1053, 1565, 1309, 1821, 1181, 1693, 1437, 1949, 1117, 1629, 1373, 1885, 1245, 1757, 1501, 989, 2013, 1085, 1597, 1341, 1853, 1213, 1725, 1469, 1981, 1149, 1661, 1405, 1917, 1277, 1789, 1533, 2045, 1539, 1283, 1795, 1155, 1667, 1411, 1923, 1091, 1603, 1347, 1859, 1219, 1731, 1475, 1987, 1571, 1315, 1827, 1187, 1699, 1443, 1955, 1635, 1379, 1891, 1251, 1763, 1507, 2019, 1555, 1299, 1811, 1683, 1427, 1939, 1619, 1363, 1875, 1235, 1747, 1491, 2003, 1587, 1331, 1843, 1715, 1459, 1971, 1651, 1395, 1907, 1779, 1523, 2035, 1547, 1803, 1675, 1419, 1931, 1611, 1355, 1867, 1739, 1483, 1995, 1579, 1835, 1707, 1451, 1963, 1643, 1899, 1771, 1515, 2027, 1563, 1819, 1691, 1947, 1627, 1883, 1755, 1499, 2011, 1595, 1851, 1723, 1979, 1659, 1915, 1787, 2043, 1799, 1671, 1927, 1607, 1863, 1735, 1991, 1831, 1703, 1959, 1895, 1767, 2023, 1815, 1943, 1879, 1751, 2007, 1847, 1975, 1911, 2039, 1935, 1871, 1999, 1967, 2031, 2015
};

const uint16_t yack_xbitrev_I2048[992]={
3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125, 127, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151, 153, 155, 157, 159, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189, 191, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223, 227, 229, 231, 233, 235, 237, 239, 241, 243, 245, 247, 249, 251, 253, 255, 259, 261, 263, 265, 267, 269, 271, 275, 277, 279, 281, 283, 285, 287, 291, 293, 295, 297, 299, 301, 303, 305, 307, 309, 311, 313, 315, 317, 319, 323, 325, 327, 329, 331, 333, 335, 339, 341, 343, 345, 347, 349, 351, 355, 357, 359, 361, 363, 365, 367, 369, 371, 373, 375, 377, 379, 381, 383, 387, 389, 391, 393, 395, 397, 399, 403, 405, 407, 409, 411, 413, 415, 419, 421, 423, 425, 427, 429, 431, 435, 437, 439, 441, 443, 445, 447, 451, 453, 455, 457, 459, 461, 463, 467, 469, 471, 473, 475, 477, 479, 483, 485, 487, 489, 491, 493, 495, 499, 501, 503, 505, 507, 509, 511, 515, 517, 519, 523, 525, 527, 531, 533, 535, 537, 539, 541, 543, 547, 549, 551, 553, 555, 557, 559, 563, 565, 567, 569, 571, 573, 575, 579, 581, 583, 587, 589, 591, 595, 597, 599, 601, 603, 605, 607, 611, 613, 615, 617, 619, 621, 623, 627, 629, 631, 633, 635, 637, 639, 643, 645, 647, 651, 653, 655, 659, 661, 663, 665, 667, 669, 671, 675, 677, 679, 683, 685, 687, 691, 693, 695, 697, 699, 701, 703, 707, 709, 711, 715, 717, 719, 723, 725, 727, 729, 731, 733, 735, 739, 741, 743, 747, 749, 751, 755, 757, 759, 761, 763, 765, 767, 771, 773, 775, 779, 781, 783, 787, 789, 791, 795, 797, 799, 803, 805, 807, 811, 813, 815, 819, 821, 823, 825, 827, 829, 831, 835, 837, 839, 843, 845, 847, 851, 853, 855, 859, 861, 863, 867, 869, 871, 875, 877, 879, 883, 885, 887, 889, 891, 893, 895, 899, 901, 903, 907, 909, 911, 915, 917, 919, 923, 925, 927, 931, 933, 935, 939, 941, 943, 947, 949, 951, 955, 957, 959, 963, 965, 967, 971, 973, 975, 979, 981, 983, 987, 989, 991, 995, 997, 999, 1003, 1005, 1007, 1011, 1013, 1015, 1019, 1021, 1023, 1027, 1031, 1035, 1037, 1039, 1043, 1045, 1047, 1051, 1053, 1055, 1059, 1061, 1063, 1067, 1069, 1071, 1075, 1077, 1079, 1083, 1085, 1087, 1091, 1095, 1099, 1101, 1103, 1107, 1109, 1111, 1115, 1117, 1119, 1123, 1125, 1127, 1131, 1133, 1135, 1139, 1141, 1143, 1147, 1149, 1151, 1155, 1159, 1163, 1165, 1167, 1171, 1173, 1175, 1179, 1181, 1183, 1187, 1191, 1195, 1197, 1199, 1203, 1205, 1207, 1211, 1213, 1215, 1219, 1223, 1227, 1229, 1231, 1235, 1237, 1239, 1243, 1245, 1247, 1251, 1255, 1259, 1261, 1263, 1267, 1269, 1271, 1275, 1277, 1279, 1283, 1287, 1291, 1293, 1295, 1299, 1303, 1307, 1309, 1311, 1315, 1319, 1323, 1325, 1327, 1331, 1333, 1335, 1339, 1341, 1343, 1347, 1351, 1355, 1357, 1359, 1363, 1367, 1371, 1373, 1375, 1379, 1383, 1387, 1389, 1391, 1395, 1397, 1399, 1403, 1405, 1407, 1411, 1415, 1419, 1421, 1423, 1427, 1431, 1435, 1437, 1439, 1443, 1447, 1451, 1453, 1455, 1459, 1463, 1467, 1469, 1471, 1475, 1479, 1483, 1485, 1487, 1491, 1495, 1499, 1501, 1503, 1507, 1511, 1515, 1517, 1519, 1523, 1527, 1531, 1533, 1535, 1539, 1543, 1547, 1551, 1555, 1559, 1563, 1565, 1567, 1571, 1575, 1579, 1581, 1583, 1587, 1591, 1595, 1597, 1599, 1603, 1607, 1611, 1615, 1619, 1623, 1627, 1629, 1631, 1635, 1639, 1643, 1645, 1647, 1651, 1655, 1659, 1661, 1663, 1667, 1671, 1675, 1679, 1683, 1687, 1691, 1693, 1695, 1699, 1703, 1707, 1711, 1715, 1719, 1723, 1725, 1727, 1731, 1735, 1739, 1743, 1747, 1751, 1755, 1757, 1759, 1763, 1767, 1771, 1775, 1779, 1783, 1787, 1789, 1791, 1795, 1799, 1803, 1807, 1811, 1815, 1819, 1823, 1827, 1831, 1835, 1839, 1843, 1847, 1851, 1853, 1855, 1859, 1863, 1867, 1871, 1875, 1879, 1883, 1887, 1891, 1895, 1899, 1903, 1907, 1911, 1915, 1917, 1919, 1923, 1927, 1931, 1935, 1939, 1943, 1947, 1951, 1955, 1959, 1963, 1967, 1971, 1975, 1979, 1983, 1987, 1991, 1995, 1999, 2003, 2007, 2011, 2015, 2019, 2023, 2027, 2031, 2035, 2039, 2043, 2047, 2055, 2059, 2063, 2067, 2071, 2075, 2079, 2083, 2087, 2091, 2095, 2099, 2103, 2107, 2111, 2119, 2123, 2127, 2131, 2135, 2139, 2143, 2147, 2151, 2155, 2159, 2163, 2167, 2171, 2175, 2183, 2187, 2191, 2195, 2199, 2203, 2207, 2215, 2219, 2223, 2227, 2231, 2235, 2239, 2247, 2251, 2255, 2259, 2263, 2267, 2271, 2279, 2283, 2287, 2291, 2295, 2299, 2303, 2311, 2315, 2319, 2327, 2331, 2335, 2343, 2347, 2351, 2355, 2359, 2363, 2367, 2375, 2379, 2383, 2391, 2395, 2399, 2407, 2411, 2415, 2419, 2423, 2427, 2431, 2439, 2443, 2447, 2455, 2459, 2463, 2471, 2475, 2479, 2487, 2491, 2495, 2503, 2507, 2511, 2519, 2523, 2527, 2535, 2539, 2543, 2551, 2555, 2559, 2567, 2575, 2583, 2587, 2591, 2599, 2603, 2607, 2615, 2619, 2623, 2631, 2639, 2647, 2651, 2655, 2663, 2667, 2671, 2679, 2683, 2687, 2695, 2703, 2711, 2715, 2719, 2727, 2735, 2743, 2747, 2751, 2759, 2767, 2775, 2779, 2783, 2791, 2799, 2807, 2811, 2815, 2823, 2831, 2839, 2847, 2855, 2863, 2871, 2875, 2879, 2887, 2895, 2903, 2911, 2919, 2927, 2935, 2939, 2943, 2951, 2959, 2967, 2975, 2983, 2991, 2999, 3007, 3015, 3023, 3031, 3039, 3047, 3055, 3063, 3071, 3087, 3095, 3103, 3111, 3119, 3127, 3135, 3151, 3159, 3167, 3175, 3183, 3191, 3199, 3215, 3223, 3231, 3247, 3255, 3263, 3279, 3287, 3295, 3311, 3319, 3327, 3343, 3359, 3375, 3383, 3391, 3407, 3423, 3439, 3447, 3455, 3471, 3487, 3503, 3519, 3535, 3551, 3567, 3583, 3615, 3631, 3647, 3679, 3695, 3711, 3743, 3775, 3807, 3839, 3903, 3967
};

const uint16_t yack_xbitrev_J2048[992]={
2049, 1025, 3073, 513, 2561, 1537, 3585, 257, 2305, 1281, 3329, 769, 2817, 1793, 3841, 129, 2177, 1153, 3201, 641, 2689, 1665, 3713, 385, 2433, 1409, 3457, 897, 2945, 1921, 3969, 2113, 1089, 3137, 577, 2625, 1601, 3649, 321, 2369, 1345, 3393, 833, 2881, 1857, 3905, 193, 2241, 1217, 3265, 705, 2753, 1729, 3777, 449, 2497, 1473, 3521, 961, 3009, 1985, 4033, 2081, 1057, 3105, 545, 2593, 1569, 3617, 289, 2337, 1313, 3361, 801, 2849, 1825, 3873, 2209, 1185, 3233, 673, 2721, 1697, 3745, 417, 2465, 1441, 3489, 929, 2977, 1953, 4001, 2145, 1121, 3169, 609, 2657, 1633, 3681, 353, 2401, 1377, 3425, 865, 2913, 1889, 3937, 2273, 1249, 3297, 737, 2785, 1761, 3809, 481, 2529, 1505, 3553, 993, 3041, 2017, 4065, 2065, 1041, 3089, 529, 2577, 1553, 3601, 2321, 1297, 3345, 785, 2833, 1809, 3857, 2193, 1169, 3217, 657, 2705, 1681, 3729, 401, 2449, 1425, 3473, 913, 2961, 1937, 3985, 2129, 1105, 3153, 593, 2641, 1617, 3665, 2385, 1361, 3409, 849, 2897, 1873, 3921, 2257, 1233, 3281, 721, 2769, 1745, 3793, 465, 2513, 1489, 3537, 977, 3025, 2001, 4049, 2097, 1073, 3121, 561, 2609, 1585, 3633, 2353, 1329, 3377, 817, 2865, 1841, 3889, 2225, 1201, 3249, 689, 2737, 1713, 3761, 2481, 1457, 3505, 945, 2993, 1969, 4017, 2161, 1137, 3185, 625, 2673, 1649, 3697, 2417, 1393, 3441, 881, 2929, 1905, 3953, 2289, 1265, 3313, 753, 2801, 1777, 3825, 2545, 1521, 3569, 1009, 3057, 2033, 4081, 2057, 1033, 3081, 2569, 1545, 3593, 2313, 1289, 3337, 777, 2825, 1801, 3849, 2185, 1161, 3209, 649, 2697, 1673, 3721, 2441, 1417, 3465, 905, 2953, 1929, 3977, 2121, 1097, 3145, 2633, 1609, 3657, 2377, 1353, 3401, 841, 2889, 1865, 3913, 2249, 1225, 3273, 713, 2761, 1737, 3785, 2505, 1481, 3529, 969, 3017, 1993, 4041, 2089, 1065, 3113, 2601, 1577, 3625, 2345, 1321, 3369, 809, 2857, 1833, 3881, 2217, 1193, 3241, 2729, 1705, 3753, 2473, 1449, 3497, 937, 2985, 1961, 4009, 2153, 1129, 3177, 2665, 1641, 3689, 2409, 1385, 3433, 873, 2921, 1897, 3945, 2281, 1257, 3305, 2793, 1769, 3817, 2537, 1513, 3561, 1001, 3049, 2025, 4073, 2073, 1049, 3097, 2585, 1561, 3609, 2329, 1305, 3353, 2841, 1817, 3865, 2201, 1177, 3225, 2713, 1689, 3737, 2457, 1433, 3481, 921, 2969, 1945, 3993, 2137, 1113, 3161, 2649, 1625, 3673, 2393, 1369, 3417, 2905, 1881, 3929, 2265, 1241, 3289, 2777, 1753, 3801, 2521, 1497, 3545, 985, 3033, 2009, 4057, 2105, 1081, 3129, 2617, 1593, 3641, 2361, 1337, 3385, 2873, 1849, 3897, 2233, 1209, 3257, 2745, 1721, 3769, 2489, 1465, 3513, 3001, 1977, 4025, 2169, 1145, 3193, 2681, 1657, 3705, 2425, 1401, 3449, 2937, 1913, 3961, 2297, 1273, 3321, 2809, 1785, 3833, 2553, 1529, 3577, 3065, 2041, 4089, 2053, 3077, 2565, 1541, 3589, 2309, 1285, 3333, 2821, 1797, 3845, 2181, 1157, 3205, 2693, 1669, 3717, 2437, 1413, 3461, 2949, 1925, 3973, 2117, 3141, 2629, 1605, 3653, 2373, 1349, 3397, 2885, 1861, 3909, 2245, 1221, 3269, 2757, 1733, 3781, 2501, 1477, 3525, 3013, 1989, 4037, 2085, 3109, 2597, 1573, 3621, 2341, 1317, 3365, 2853, 1829, 3877, 2213, 3237, 2725, 1701, 3749, 2469, 1445, 3493, 2981, 1957, 4005, 2149, 3173, 2661, 1637, 3685, 2405, 1381, 3429, 2917, 1893, 3941, 2277, 3301, 2789, 1765, 3813, 2533, 1509, 3557, 3045, 2021, 4069, 2069, 3093, 2581, 1557, 3605, 2325, 3349, 2837, 1813, 3861, 2197, 3221, 2709, 1685, 3733, 2453, 1429, 3477, 2965, 1941, 3989, 2133, 3157, 2645, 1621, 3669, 2389, 3413, 2901, 1877, 3925, 2261, 3285, 2773, 1749, 3797, 2517, 1493, 3541, 3029, 2005, 4053, 2101, 3125, 2613, 1589, 3637, 2357, 3381, 2869, 1845, 3893, 2229, 3253, 2741, 1717, 3765, 2485, 3509, 2997, 1973, 4021, 2165, 3189, 2677, 1653, 3701, 2421, 3445, 2933, 1909, 3957, 2293, 3317, 2805, 1781, 3829, 2549, 3573, 3061, 2037, 4085, 2061, 3085, 2573, 3597, 2317, 3341, 2829, 1805, 3853, 2189, 3213, 2701, 1677, 3725, 2445, 3469, 2957, 1933, 3981, 2125, 3149, 2637, 3661, 2381, 3405, 2893, 1869, 3917, 2253, 3277, 2765, 1741, 3789, 2509, 3533, 3021, 1997, 4045, 2093, 3117, 2605, 3629, 2349, 3373, 2861, 1837, 3885, 2221, 3245, 2733, 3757, 2477, 3501, 2989, 1965, 4013, 2157, 3181, 2669, 3693, 2413, 3437, 2925, 1901, 3949, 2285, 3309, 2797, 3821, 2541, 3565, 3053, 2029, 4077, 2077, 3101, 2589, 3613, 2333, 3357, 2845, 3869, 2205, 3229, 2717, 3741, 2461, 3485, 2973, 1949, 3997, 2141, 3165, 2653, 3677, 2397, 3421, 2909, 3933, 2269, 3293, 2781, 3805, 2525, 3549, 3037, 2013, 4061, 2109, 3133, 2621, 3645, 2365, 3389, 2877, 3901, 2237, 3261, 2749, 3773, 2493, 3517, 3005, 4029, 2173, 3197, 2685, 3709, 2429, 3453, 2941, 3965, 2301, 3325, 2813, 3837, 2557, 3581, 3069, 4093, 3075, 2563, 3587, 2307, 3331, 2819, 3843, 2179, 3203, 2691, 3715, 2435, 3459, 2947, 3971, 3139, 2627, 3651, 2371, 3395, 2883, 3907, 2243, 3267, 2755, 3779, 2499, 3523, 3011, 4035, 3107, 2595, 3619, 2339, 3363, 2851, 3875, 3235, 2723, 3747, 2467, 3491, 2979, 4003, 3171, 2659, 3683, 2403, 3427, 2915, 3939, 3299, 2787, 3811, 2531, 3555, 3043, 4067, 3091, 2579, 3603, 3347, 2835, 3859, 3219, 2707, 3731, 2451, 3475, 2963, 3987, 3155, 2643, 3667, 3411, 2899, 3923, 3283, 2771, 3795, 2515, 3539, 3027, 4051, 3123, 2611, 3635, 3379, 2867, 3891, 3251, 2739, 3763, 3507, 2995, 4019, 3187, 2675, 3699, 3443, 2931, 3955, 3315, 2803, 3827, 3571, 3059, 4083, 3083, 3595, 3339, 2827, 3851, 3211, 2699, 3723, 3467, 2955, 3979, 3147, 3659, 3403, 2891, 3915, 3275, 2763, 3787, 3531, 3019, 4043, 3115, 3627, 3371, 2859, 3883, 3243, 3755, 3499, 2987, 4011, 3179, 3691, 3435, 2923, 3947, 3307, 3819, 3563, 3051, 4075, 3099, 3611, 3355, 3867, 3227, 3739, 3483, 2971, 3995, 3163, 3675, 3419, 3931, 3291, 3803, 3547, 3035, 4059, 3131, 3643, 3387, 3899, 3259, 3771, 3515, 4027, 3195, 3707, 3451, 3963, 3323, 3835, 3579, 4091, 3591, 3335, 3847, 3207, 3719, 3463, 3975, 3655, 3399, 3911, 3271, 3783, 3527, 4039, 3623, 3367, 3879, 3751, 3495, 4007, 3687, 3431, 3943, 3815, 3559, 4071, 3607, 3863, 3735, 3479, 3991, 3671, 3927, 3799, 3543, 4055, 3639, 3895, 3767, 4023, 3703, 3959, 3831, 4087, 3855, 3727, 3983, 3919, 3791, 4047, 3887, 4015, 3951, 4079, 3999, 4063
};

