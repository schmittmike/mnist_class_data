/***********************************
** filename: hand_try5.c
** Department of Electronics
** Carleton University
***********************************/

#include<math.h>

int hand_try5(float *input, float *output)
{
     /*declare the variables used in the function*/
     float x[2];
     float z[5];
     float y[1];
     float xtmp, xminTmp, xmaxTmp, yminTmp, ymaxTmp;

     /*input scaling*/
     x[0]=-1.0+(2.0)*(input[0]-(5514.88)) / ((28691.4) - (5514.88));
     x[1]=-1.0+(2.0)*(input[1]-(22.0)) / ((239.0) - (22.0));

     /*calculating hidden neurons*/
     z[0] = 1.0 / ( 1.0 + exp(-1.0 * (-3.90524+x[0]*(1.0154)+x[1]*(-1.90856))));
     z[1] = 1.0 / ( 1.0 + exp(-1.0 * (-2.97126+x[0]*(1.02579)+x[1]*(0.786421))));
     z[2] = 1.0 / ( 1.0 + exp(-1.0 * (-3.26237+x[0]*(-0.0478322)+x[1]*(0.591173))));
     z[3] = 1.0 / ( 1.0 + exp(-1.0 * (-3.23043+x[0]*(0.163303)+x[1]*(0.576706))));
     z[4] = 1.0 / ( 1.0 + exp(-1.0 * (-4.86683+x[0]*(6.32204)+x[1]*(-16.1359))));

     /*calculating output neurons*/
     y[0] = 0.00333253+z[0]*(-0.979725)+z[1]*(0.270113)+z[2]*(-0.10409)+z[3]*(-0.0754795)+z[4]*(1.06785);

     /*output scaling*/
     output[0] = 0.0+(y[0]-(0.0))*((1.0) - (0.0))/((1.0)-(0.0));

     /*checking if the input values are within the model effective range*/
     if (input[0] > 28691.4) return 1;
     if (input[0] < 5514.88) return -1;
     if (input[1] > 239.0) return 2;
     if (input[1] < 22.0) return -2;
     return 0;

}
