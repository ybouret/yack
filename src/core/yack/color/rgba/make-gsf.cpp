#include "yack/color/rgba/make-gsf.hpp"

namespace yack
{
    namespace color
    {
        make_gsf:: make_gsf() throw() :
        rgba_to_data( sizeof(float) )
        {
        }

        make_gsf:: ~make_gsf() throw()
        {
        }

        void make_gsf:: operator()(void *addr, const rgba &c) throw()
        {
            assert(addr);
            *static_cast<float *>(addr) = table[c.r+c.g+c.b];
        }

        const float make_gsf:: table[] =
        {
            (0.0f/765),
            (1.0f/765),(2.0f/765),(3.0f/765),(4.0f/765),(5.0f/765),(6.0f/765),(7.0f/765),(8.0f/765),
            (9.0f/765),(10.0f/765),(11.0f/765),(12.0f/765),(13.0f/765),(14.0f/765),(15.0f/765),(16.0f/765),
            (17.0f/765),(18.0f/765),(19.0f/765),(20.0f/765),(21.0f/765),(22.0f/765),(23.0f/765),(24.0f/765),
            (25.0f/765),(26.0f/765),(27.0f/765),(28.0f/765),(29.0f/765),(30.0f/765),(31.0f/765),(32.0f/765),
            (33.0f/765),(34.0f/765),(35.0f/765),(36.0f/765),(37.0f/765),(38.0f/765),(39.0f/765),(40.0f/765),
            (41.0f/765),(42.0f/765),(43.0f/765),(44.0f/765),(45.0f/765),(46.0f/765),(47.0f/765),(48.0f/765),
            (49.0f/765),(50.0f/765),(51.0f/765),(52.0f/765),(53.0f/765),(54.0f/765),(55.0f/765),(56.0f/765),
            (57.0f/765),(58.0f/765),(59.0f/765),(60.0f/765),(61.0f/765),(62.0f/765),(63.0f/765),(64.0f/765),
            (65.0f/765),(66.0f/765),(67.0f/765),(68.0f/765),(69.0f/765),(70.0f/765),(71.0f/765),(72.0f/765),
            (73.0f/765),(74.0f/765),(75.0f/765),(76.0f/765),(77.0f/765),(78.0f/765),(79.0f/765),(80.0f/765),
            (81.0f/765),(82.0f/765),(83.0f/765),(84.0f/765),(85.0f/765),(86.0f/765),(87.0f/765),(88.0f/765),
            (89.0f/765),(90.0f/765),(91.0f/765),(92.0f/765),(93.0f/765),(94.0f/765),(95.0f/765),(96.0f/765),
            (97.0f/765),(98.0f/765),(99.0f/765),(100.0f/765),(101.0f/765),(102.0f/765),(103.0f/765),(104.0f/765),
            (105.0f/765),(106.0f/765),(107.0f/765),(108.0f/765),(109.0f/765),(110.0f/765),(111.0f/765),(112.0f/765),
            (113.0f/765),(114.0f/765),(115.0f/765),(116.0f/765),(117.0f/765),(118.0f/765),(119.0f/765),(120.0f/765),
            (121.0f/765),(122.0f/765),(123.0f/765),(124.0f/765),(125.0f/765),(126.0f/765),(127.0f/765),(128.0f/765),
            (129.0f/765),(130.0f/765),(131.0f/765),(132.0f/765),(133.0f/765),(134.0f/765),(135.0f/765),(136.0f/765),
            (137.0f/765),(138.0f/765),(139.0f/765),(140.0f/765),(141.0f/765),(142.0f/765),(143.0f/765),(144.0f/765),
            (145.0f/765),(146.0f/765),(147.0f/765),(148.0f/765),(149.0f/765),(150.0f/765),(151.0f/765),(152.0f/765),
            (153.0f/765),(154.0f/765),(155.0f/765),(156.0f/765),(157.0f/765),(158.0f/765),(159.0f/765),(160.0f/765),
            (161.0f/765),(162.0f/765),(163.0f/765),(164.0f/765),(165.0f/765),(166.0f/765),(167.0f/765),(168.0f/765),
            (169.0f/765),(170.0f/765),(171.0f/765),(172.0f/765),(173.0f/765),(174.0f/765),(175.0f/765),(176.0f/765),
            (177.0f/765),(178.0f/765),(179.0f/765),(180.0f/765),(181.0f/765),(182.0f/765),(183.0f/765),(184.0f/765),
            (185.0f/765),(186.0f/765),(187.0f/765),(188.0f/765),(189.0f/765),(190.0f/765),(191.0f/765),(192.0f/765),
            (193.0f/765),(194.0f/765),(195.0f/765),(196.0f/765),(197.0f/765),(198.0f/765),(199.0f/765),(200.0f/765),
            (201.0f/765),(202.0f/765),(203.0f/765),(204.0f/765),(205.0f/765),(206.0f/765),(207.0f/765),(208.0f/765),
            (209.0f/765),(210.0f/765),(211.0f/765),(212.0f/765),(213.0f/765),(214.0f/765),(215.0f/765),(216.0f/765),
            (217.0f/765),(218.0f/765),(219.0f/765),(220.0f/765),(221.0f/765),(222.0f/765),(223.0f/765),(224.0f/765),
            (225.0f/765),(226.0f/765),(227.0f/765),(228.0f/765),(229.0f/765),(230.0f/765),(231.0f/765),(232.0f/765),
            (233.0f/765),(234.0f/765),(235.0f/765),(236.0f/765),(237.0f/765),(238.0f/765),(239.0f/765),(240.0f/765),
            (241.0f/765),(242.0f/765),(243.0f/765),(244.0f/765),(245.0f/765),(246.0f/765),(247.0f/765),(248.0f/765),
            (249.0f/765),(250.0f/765),(251.0f/765),(252.0f/765),(253.0f/765),(254.0f/765),(255.0f/765),(256.0f/765),
            (257.0f/765),(258.0f/765),(259.0f/765),(260.0f/765),(261.0f/765),(262.0f/765),(263.0f/765),(264.0f/765),
            (265.0f/765),(266.0f/765),(267.0f/765),(268.0f/765),(269.0f/765),(270.0f/765),(271.0f/765),(272.0f/765),
            (273.0f/765),(274.0f/765),(275.0f/765),(276.0f/765),(277.0f/765),(278.0f/765),(279.0f/765),(280.0f/765),
            (281.0f/765),(282.0f/765),(283.0f/765),(284.0f/765),(285.0f/765),(286.0f/765),(287.0f/765),(288.0f/765),
            (289.0f/765),(290.0f/765),(291.0f/765),(292.0f/765),(293.0f/765),(294.0f/765),(295.0f/765),(296.0f/765),
            (297.0f/765),(298.0f/765),(299.0f/765),(300.0f/765),(301.0f/765),(302.0f/765),(303.0f/765),(304.0f/765),
            (305.0f/765),(306.0f/765),(307.0f/765),(308.0f/765),(309.0f/765),(310.0f/765),(311.0f/765),(312.0f/765),
            (313.0f/765),(314.0f/765),(315.0f/765),(316.0f/765),(317.0f/765),(318.0f/765),(319.0f/765),(320.0f/765),
            (321.0f/765),(322.0f/765),(323.0f/765),(324.0f/765),(325.0f/765),(326.0f/765),(327.0f/765),(328.0f/765),
            (329.0f/765),(330.0f/765),(331.0f/765),(332.0f/765),(333.0f/765),(334.0f/765),(335.0f/765),(336.0f/765),
            (337.0f/765),(338.0f/765),(339.0f/765),(340.0f/765),(341.0f/765),(342.0f/765),(343.0f/765),(344.0f/765),
            (345.0f/765),(346.0f/765),(347.0f/765),(348.0f/765),(349.0f/765),(350.0f/765),(351.0f/765),(352.0f/765),
            (353.0f/765),(354.0f/765),(355.0f/765),(356.0f/765),(357.0f/765),(358.0f/765),(359.0f/765),(360.0f/765),
            (361.0f/765),(362.0f/765),(363.0f/765),(364.0f/765),(365.0f/765),(366.0f/765),(367.0f/765),(368.0f/765),
            (369.0f/765),(370.0f/765),(371.0f/765),(372.0f/765),(373.0f/765),(374.0f/765),(375.0f/765),(376.0f/765),
            (377.0f/765),(378.0f/765),(379.0f/765),(380.0f/765),(381.0f/765),(382.0f/765),(383.0f/765),(384.0f/765),
            (385.0f/765),(386.0f/765),(387.0f/765),(388.0f/765),(389.0f/765),(390.0f/765),(391.0f/765),(392.0f/765),
            (393.0f/765),(394.0f/765),(395.0f/765),(396.0f/765),(397.0f/765),(398.0f/765),(399.0f/765),(400.0f/765),
            (401.0f/765),(402.0f/765),(403.0f/765),(404.0f/765),(405.0f/765),(406.0f/765),(407.0f/765),(408.0f/765),
            (409.0f/765),(410.0f/765),(411.0f/765),(412.0f/765),(413.0f/765),(414.0f/765),(415.0f/765),(416.0f/765),
            (417.0f/765),(418.0f/765),(419.0f/765),(420.0f/765),(421.0f/765),(422.0f/765),(423.0f/765),(424.0f/765),
            (425.0f/765),(426.0f/765),(427.0f/765),(428.0f/765),(429.0f/765),(430.0f/765),(431.0f/765),(432.0f/765),
            (433.0f/765),(434.0f/765),(435.0f/765),(436.0f/765),(437.0f/765),(438.0f/765),(439.0f/765),(440.0f/765),
            (441.0f/765),(442.0f/765),(443.0f/765),(444.0f/765),(445.0f/765),(446.0f/765),(447.0f/765),(448.0f/765),
            (449.0f/765),(450.0f/765),(451.0f/765),(452.0f/765),(453.0f/765),(454.0f/765),(455.0f/765),(456.0f/765),
            (457.0f/765),(458.0f/765),(459.0f/765),(460.0f/765),(461.0f/765),(462.0f/765),(463.0f/765),(464.0f/765),
            (465.0f/765),(466.0f/765),(467.0f/765),(468.0f/765),(469.0f/765),(470.0f/765),(471.0f/765),(472.0f/765),
            (473.0f/765),(474.0f/765),(475.0f/765),(476.0f/765),(477.0f/765),(478.0f/765),(479.0f/765),(480.0f/765),
            (481.0f/765),(482.0f/765),(483.0f/765),(484.0f/765),(485.0f/765),(486.0f/765),(487.0f/765),(488.0f/765),
            (489.0f/765),(490.0f/765),(491.0f/765),(492.0f/765),(493.0f/765),(494.0f/765),(495.0f/765),(496.0f/765),
            (497.0f/765),(498.0f/765),(499.0f/765),(500.0f/765),(501.0f/765),(502.0f/765),(503.0f/765),(504.0f/765),
            (505.0f/765),(506.0f/765),(507.0f/765),(508.0f/765),(509.0f/765),(510.0f/765),(511.0f/765),(512.0f/765),
            (513.0f/765),(514.0f/765),(515.0f/765),(516.0f/765),(517.0f/765),(518.0f/765),(519.0f/765),(520.0f/765),
            (521.0f/765),(522.0f/765),(523.0f/765),(524.0f/765),(525.0f/765),(526.0f/765),(527.0f/765),(528.0f/765),
            (529.0f/765),(530.0f/765),(531.0f/765),(532.0f/765),(533.0f/765),(534.0f/765),(535.0f/765),(536.0f/765),
            (537.0f/765),(538.0f/765),(539.0f/765),(540.0f/765),(541.0f/765),(542.0f/765),(543.0f/765),(544.0f/765),
            (545.0f/765),(546.0f/765),(547.0f/765),(548.0f/765),(549.0f/765),(550.0f/765),(551.0f/765),(552.0f/765),
            (553.0f/765),(554.0f/765),(555.0f/765),(556.0f/765),(557.0f/765),(558.0f/765),(559.0f/765),(560.0f/765),
            (561.0f/765),(562.0f/765),(563.0f/765),(564.0f/765),(565.0f/765),(566.0f/765),(567.0f/765),(568.0f/765),
            (569.0f/765),(570.0f/765),(571.0f/765),(572.0f/765),(573.0f/765),(574.0f/765),(575.0f/765),(576.0f/765),
            (577.0f/765),(578.0f/765),(579.0f/765),(580.0f/765),(581.0f/765),(582.0f/765),(583.0f/765),(584.0f/765),
            (585.0f/765),(586.0f/765),(587.0f/765),(588.0f/765),(589.0f/765),(590.0f/765),(591.0f/765),(592.0f/765),
            (593.0f/765),(594.0f/765),(595.0f/765),(596.0f/765),(597.0f/765),(598.0f/765),(599.0f/765),(600.0f/765),
            (601.0f/765),(602.0f/765),(603.0f/765),(604.0f/765),(605.0f/765),(606.0f/765),(607.0f/765),(608.0f/765),
            (609.0f/765),(610.0f/765),(611.0f/765),(612.0f/765),(613.0f/765),(614.0f/765),(615.0f/765),(616.0f/765),
            (617.0f/765),(618.0f/765),(619.0f/765),(620.0f/765),(621.0f/765),(622.0f/765),(623.0f/765),(624.0f/765),
            (625.0f/765),(626.0f/765),(627.0f/765),(628.0f/765),(629.0f/765),(630.0f/765),(631.0f/765),(632.0f/765),
            (633.0f/765),(634.0f/765),(635.0f/765),(636.0f/765),(637.0f/765),(638.0f/765),(639.0f/765),(640.0f/765),
            (641.0f/765),(642.0f/765),(643.0f/765),(644.0f/765),(645.0f/765),(646.0f/765),(647.0f/765),(648.0f/765),
            (649.0f/765),(650.0f/765),(651.0f/765),(652.0f/765),(653.0f/765),(654.0f/765),(655.0f/765),(656.0f/765),
            (657.0f/765),(658.0f/765),(659.0f/765),(660.0f/765),(661.0f/765),(662.0f/765),(663.0f/765),(664.0f/765),
            (665.0f/765),(666.0f/765),(667.0f/765),(668.0f/765),(669.0f/765),(670.0f/765),(671.0f/765),(672.0f/765),
            (673.0f/765),(674.0f/765),(675.0f/765),(676.0f/765),(677.0f/765),(678.0f/765),(679.0f/765),(680.0f/765),
            (681.0f/765),(682.0f/765),(683.0f/765),(684.0f/765),(685.0f/765),(686.0f/765),(687.0f/765),(688.0f/765),
            (689.0f/765),(690.0f/765),(691.0f/765),(692.0f/765),(693.0f/765),(694.0f/765),(695.0f/765),(696.0f/765),
            (697.0f/765),(698.0f/765),(699.0f/765),(700.0f/765),(701.0f/765),(702.0f/765),(703.0f/765),(704.0f/765),
            (705.0f/765),(706.0f/765),(707.0f/765),(708.0f/765),(709.0f/765),(710.0f/765),(711.0f/765),(712.0f/765),
            (713.0f/765),(714.0f/765),(715.0f/765),(716.0f/765),(717.0f/765),(718.0f/765),(719.0f/765),(720.0f/765),
            (721.0f/765),(722.0f/765),(723.0f/765),(724.0f/765),(725.0f/765),(726.0f/765),(727.0f/765),(728.0f/765),
            (729.0f/765),(730.0f/765),(731.0f/765),(732.0f/765),(733.0f/765),(734.0f/765),(735.0f/765),(736.0f/765),
            (737.0f/765),(738.0f/765),(739.0f/765),(740.0f/765),(741.0f/765),(742.0f/765),(743.0f/765),(744.0f/765),
            (745.0f/765),(746.0f/765),(747.0f/765),(748.0f/765),(749.0f/765),(750.0f/765),(751.0f/765),(752.0f/765),
            (753.0f/765),(754.0f/765),(755.0f/765),(756.0f/765),(757.0f/765),(758.0f/765),(759.0f/765),(760.0f/765),
            (761.0f/765),(762.0f/765),(763.0f/765),(764.0f/765),(765.0f/765)};


    }

}

