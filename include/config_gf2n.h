#ifndef _CONFIG_GF2N_H
#define _CONFIG_GF2N_H

/* Irreducible polynomial defining GF(2^n), n=HFEn: either the trinomial
   f(x) = x^n + x^K3 + 1 or the pentanomial
   f(x) = x^n + x^K3 + x^K2 + x^K1 + 1, with 0 < K1 < K2 < K3 <= 32.
   For a trinomial only K3 is defined. To use another n, add an entry. */

#include "config_HFE.h"

#if HFEn==65
    #define K3 18
#elif HFEn==66
    #define K3 3
#elif HFEn==67
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==68
    #define K3 9
#elif HFEn==69
    #define K3 10
    #define K2 4
    #define K1 1
#elif HFEn==70
    #define K3 5
    #define K2 3
    #define K1 1
#elif HFEn==71
    #define K3 6
#elif HFEn==72
    #define K3 16
    #define K2 7
    #define K1 1
#elif HFEn==73
    #define K3 25
#elif HFEn==74
    #define K3 6
    #define K2 2
    #define K1 1
#elif HFEn==75
    #define K3 6
    #define K2 3
    #define K1 1
#elif HFEn==76
    #define K3 21
#elif HFEn==77
    #define K3 10
    #define K2 2
    #define K1 1
#elif HFEn==78
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==79
    #define K3 9
#elif HFEn==80
    #define K3 14
    #define K2 13
    #define K1 1
#elif HFEn==81
    #define K3 4
#elif HFEn==82
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==83
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==84
    #define K3 5
#elif HFEn==85
    #define K3 8
    #define K2 2
    #define K1 1
#elif HFEn==86
    #define K3 21
#elif HFEn==87
    #define K3 13
#elif HFEn==88
    #define K3 9
    #define K2 3
    #define K1 1
#elif HFEn==89
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==90
    #define K3 27
#elif HFEn==91
    #define K3 8
    #define K2 5
    #define K1 1
#elif HFEn==92
    #define K3 21
#elif HFEn==93
    #define K3 2
#elif HFEn==94
    #define K3 21
#elif HFEn==95
    #define K3 11
#elif HFEn==96
    #define K3 16
    #define K2 3
    #define K1 1
#elif HFEn==97
    #define K3 6
#elif HFEn==98
    #define K3 11
#elif HFEn==99
    #define K3 6
    #define K2 3
    #define K1 1
#elif HFEn==100
    #define K3 15
#elif HFEn==101
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==102
    #define K3 29
#elif HFEn==103
    #define K3 9
#elif HFEn==104
    #define K3 4
    #define K2 3
    #define K1 1
#elif HFEn==105
    #define K3 4
#elif HFEn==106
    #define K3 15
#elif HFEn==107
    #define K3 16
    #define K2 6
    #define K1 1
#elif HFEn==108
    #define K3 17
#elif HFEn==109
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==110
    #define K3 6
    #define K2 4
    #define K1 1
#elif HFEn==111
    #define K3 10
#elif HFEn==112
    #define K3 15
    #define K2 10
    #define K1 1
#elif HFEn==113
    #define K3 9
#elif HFEn==114
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==115
    #define K3 12
    #define K2 6
    #define K1 1
#elif HFEn==116
    #define K3 4
    #define K2 2
    #define K1 1
#elif HFEn==117
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==118
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==119
    #define K3 8
#elif HFEn==120
    #define K3 4
    #define K2 3
    #define K1 1
#elif HFEn==121
    #define K3 18
#elif HFEn==122
    #define K3 6
    #define K2 2
    #define K1 1
#elif HFEn==123
    #define K3 2
#elif HFEn==124
    #define K3 19
#elif HFEn==125
    #define K3 18
    #define K2 5
    #define K1 1
#elif HFEn==126
    #define K3 21
#elif HFEn==127
    #define K3 1
#elif HFEn==128
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==129
    #define K3 5
#elif HFEn==130
    #define K3 3
#elif HFEn==131
    #define K3 13
    #define K2 2
    #define K1 1
#elif HFEn==132
    #define K3 17
#elif HFEn==133
    #define K3 18
    #define K2 15
    #define K1 1
#elif HFEn==134
    #define K3 7
    #define K2 5
    #define K1 1
#elif HFEn==135
    #define K3 11
#elif HFEn==136
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==137
    #define K3 21
#elif HFEn==138
    #define K3 8
    #define K2 7
    #define K1 1
#elif HFEn==139
    #define K3 14
    #define K2 8
    #define K1 1
#elif HFEn==140
    #define K3 15
#elif HFEn==141
    #define K3 10
    #define K2 4
    #define K1 1
#elif HFEn==142
    #define K3 21
#elif HFEn==143
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==144
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==145
    #define K3 6
    #define K2 5
    #define K1 1
#elif HFEn==146
    #define K3 9
    #define K2 4
    #define K1 1
#elif HFEn==147
    #define K3 14
#elif HFEn==148
    #define K3 27
#elif HFEn==149
    #define K3 18
    #define K2 8
    #define K1 1
#elif HFEn==150
    #define K3 8
    #define K2 4
    #define K1 1
#elif HFEn==151
    #define K3 3
#elif HFEn==152
    #define K3 16
    #define K2 11
    #define K1 1
#elif HFEn==153
    #define K3 1
#elif HFEn==154
    #define K3 15
#elif HFEn==155
    #define K3 18
    #define K2 10
    #define K1 1
#elif HFEn==156
    #define K3 9
#elif HFEn==157
    #define K3 16
    #define K2 3
    #define K1 1
#elif HFEn==158
    #define K3 15
    #define K2 8
    #define K1 1
#elif HFEn==159
    #define K3 31
#elif HFEn==160
    #define K3 7
    #define K2 3
    #define K1 1
#elif HFEn==161
    #define K3 18
#elif HFEn==162
    #define K3 27
#elif HFEn==163
    #define K3 8
    #define K2 2
    #define K1 1
#elif HFEn==164
    #define K3 13
    #define K2 11
    #define K1 1
#elif HFEn==165
    #define K3 16
    #define K2 6
    #define K1 1
#elif HFEn==166
    #define K3 6
    #define K2 5
    #define K1 1
#elif HFEn==167
    #define K3 6
#elif HFEn==168
    #define K3 18
    #define K2 15
    #define K1 1
#elif HFEn==169
    #define K3 12
    #define K2 7
    #define K1 1
#elif HFEn==170
    #define K3 11
#elif HFEn==171
    #define K3 14
    #define K2 8
    #define K1 1
#elif HFEn==172
    #define K3 1
#elif HFEn==173
    #define K3 10
    #define K2 2
    #define K1 1
#elif HFEn==174
    #define K3 13
#elif HFEn==175
    #define K3 16
#elif HFEn==176
    #define K3 15
    #define K2 7
    #define K1 1
#elif HFEn==177
    #define K3 8
#elif HFEn==178
    #define K3 31
#elif HFEn==179
    #define K3 4
    #define K2 2
    #define K1 1
#elif HFEn==180
    #define K3 3
#elif HFEn==181
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==182
    #define K3 8
    #define K2 6
    #define K1 1
#elif HFEn==183
    #define K3 14
    #define K2 11
    #define K1 1
#elif HFEn==184
    #define K3 18
    #define K2 13
    #define K1 1
#elif HFEn==185
    #define K3 24
#elif HFEn==186
    #define K3 11
#elif HFEn==187
    #define K3 18
    #define K2 6
    #define K1 1
#elif HFEn==188
    #define K3 15
    #define K2 5
    #define K1 1
#elif HFEn==189
    #define K3 18
    #define K2 4
    #define K1 1
#elif HFEn==190
    #define K3 18
    #define K2 17
    #define K1 1
#elif HFEn==191
    #define K3 9
#elif HFEn==192
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==193
    #define K3 15
#elif HFEn==194
    #define K3 11
    #define K2 7
    #define K1 1
#elif HFEn==195
    #define K3 10
    #define K2 9
    #define K1 1
#elif HFEn==196
    #define K3 3
#elif HFEn==197
    #define K3 14
    #define K2 6
    #define K1 1
#elif HFEn==198
    #define K3 9
#elif HFEn==199
    #define K3 9
    #define K2 4
    #define K1 1
#elif HFEn==200
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==201
    #define K3 14
#elif HFEn==202
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==203
    #define K3 8
    #define K2 7
    #define K1 1
#elif HFEn==204
    #define K3 27
#elif HFEn==205
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==206
    #define K3 11
    #define K2 9
    #define K1 1
#elif HFEn==207
    #define K3 9
    #define K2 6
    #define K1 1
#elif HFEn==208
    #define K3 9
    #define K2 3
    #define K1 1
#elif HFEn==209
    #define K3 6
#elif HFEn==210
    #define K3 7
#elif HFEn==211
    #define K3 12
    #define K2 6
    #define K1 1
#elif HFEn==212
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==213
    #define K3 8
    #define K2 7
    #define K1 1
#elif HFEn==214
    #define K3 5
    #define K2 3
    #define K1 1
#elif HFEn==215
    #define K3 23
#elif HFEn==216
    #define K3 7
    #define K2 3
    #define K1 1
#elif HFEn==217
    #define K3 12
    #define K2 11
    #define K1 1
#elif HFEn==218
    #define K3 11
#elif HFEn==219
    #define K3 8
    #define K2 4
    #define K1 1
#elif HFEn==220
    #define K3 7
#elif HFEn==221
    #define K3 16
    #define K2 6
    #define K1 1
#elif HFEn==222
    #define K3 6
    #define K2 3
    #define K1 1
#elif HFEn==223
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==224
    #define K3 21
    #define K2 7
    #define K1 1
#elif HFEn==225
    #define K3 32
#elif HFEn==226
    #define K3 20
    #define K2 17
    #define K1 1
#elif HFEn==227
    #define K3 20
    #define K2 12
    #define K1 1
#elif HFEn==228
    #define K3 8
    #define K2 2
    #define K1 1
#elif HFEn==229
    #define K3 10
    #define K2 4
    #define K1 1
#elif HFEn==230
    #define K3 13
    #define K2 2
    #define K1 1
#elif HFEn==231
    #define K3 26
#elif HFEn==232
    #define K3 23
    #define K2 2
    #define K1 1
#elif HFEn==233
    #define K3 9
    #define K2 4
    #define K1 1
#elif HFEn==234
    #define K3 31
#elif HFEn==235
    #define K3 9
    #define K2 6
    #define K1 1
#elif HFEn==236
    #define K3 5
#elif HFEn==237
    #define K3 7
    #define K2 4
    #define K1 1
#elif HFEn==238
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==239
    #define K3 12
    #define K2 7
    #define K1 1
#elif HFEn==240
    #define K3 16
    #define K2 11
    #define K1 1
#elif HFEn==241
    #define K3 15
    #define K2 4
    #define K1 1
#elif HFEn==242
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==243
    #define K3 8
    #define K2 5
    #define K1 1
#elif HFEn==244
    #define K3 9
    #define K2 4
    #define K1 1
#elif HFEn==245
    #define K3 6
    #define K2 4
    #define K1 1
#elif HFEn==246
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==247
    #define K3 21
    #define K2 10
    #define K1 1
#elif HFEn==248
    #define K3 26
    #define K2 9
    #define K1 1
#elif HFEn==249
    #define K3 7
    #define K2 4
    #define K1 1
#elif HFEn==250
    #define K3 15
    #define K2 2
    #define K1 1
#elif HFEn==251
    #define K3 14
    #define K2 4
    #define K1 1
#elif HFEn==252
    #define K3 15
#elif HFEn==253
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==254
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==255
    #define K3 11
    #define K2 8
    #define K1 1
#elif HFEn==256
    #define K3 16
    #define K2 3
    #define K1 1
#elif HFEn==257
    #define K3 12
#elif HFEn==258
    #define K3 26
    #define K2 2
    #define K1 1
#elif HFEn==259
    #define K3 15
    #define K2 4
    #define K1 1
#elif HFEn==260
    #define K3 15
#elif HFEn==261
    #define K3 11
    #define K2 4
    #define K1 1
#elif HFEn==262
    #define K3 22
    #define K2 3
    #define K1 1
#elif HFEn==263
    #define K3 13
    #define K2 11
    #define K1 1
#elif HFEn==264
    #define K3 10
    #define K2 9
    #define K1 1
#elif HFEn==265
    #define K3 15
    #define K2 10
    #define K1 1
#elif HFEn==266
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==267
    #define K3 14
    #define K2 4
    #define K1 1
#elif HFEn==268
    #define K3 25
#elif HFEn==269
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==270
    #define K3 18
    #define K2 4
    #define K1 1
#elif HFEn==271
    #define K3 16
    #define K2 3
    #define K1 1
#elif HFEn==272
    #define K3 27
    #define K2 11
    #define K1 1
#elif HFEn==273
    #define K3 23
#elif HFEn==274
    #define K3 11
    #define K2 8
    #define K1 1
#elif HFEn==275
    #define K3 16
    #define K2 6
    #define K1 1
#elif HFEn==276
    #define K3 6
    #define K2 3
    #define K1 1
#elif HFEn==277
    #define K3 30
    #define K2 19
    #define K1 1
#elif HFEn==278
    #define K3 5
#elif HFEn==279
    #define K3 5
#elif HFEn==280
    #define K3 15
    #define K2 10
    #define K1 1
#elif HFEn==281
    #define K3 9
    #define K2 4
    #define K1 1
#elif HFEn==282
    #define K3 8
    #define K2 7
    #define K1 1
#elif HFEn==283
    #define K3 26
    #define K2 9
    #define K1 1
#elif HFEn==284
    #define K3 12
    #define K2 7
    #define K1 1
#elif HFEn==285
    #define K3 19
    #define K2 4
    #define K1 1
#elif HFEn==286
    #define K3 15
    #define K2 10
    #define K1 1
#elif HFEn==287
    #define K3 25
    #define K2 5
    #define K1 1
#elif HFEn==288
    #define K3 11
    #define K2 10
    #define K1 1
#elif HFEn==289
    #define K3 21
#elif HFEn==290
    #define K3 11
    #define K2 10
    #define K1 1
#elif HFEn==291
    #define K3 16
    #define K2 4
    #define K1 1
#elif HFEn==292
    #define K3 7
    #define K2 3
    #define K1 1
#elif HFEn==293
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==294
    #define K3 16
    #define K2 10
    #define K1 1
#elif HFEn==295
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==296
    #define K3 15
    #define K2 8
    #define K1 1
#elif HFEn==297
    #define K3 5
#elif HFEn==298
    #define K3 23
    #define K2 15
    #define K1 1
#elif HFEn==299
    #define K3 18
    #define K2 14
    #define K1 1
#elif HFEn==300
    #define K3 5
#elif HFEn==301
    #define K3 11
    #define K2 8
    #define K1 1
#elif HFEn==302
    #define K3 16
    #define K2 14
    #define K1 1
#elif HFEn==303
    #define K3 1
#elif HFEn==304
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==305
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==306
    #define K3 7
    #define K2 3
    #define K1 1
#elif HFEn==307
    #define K3 25
    #define K2 10
    #define K1 1
#elif HFEn==308
    #define K3 15
#elif HFEn==309
    #define K3 18
    #define K2 10
    #define K1 1
#elif HFEn==310
    #define K3 8
    #define K2 5
    #define K1 1
#elif HFEn==311
    #define K3 13
    #define K2 5
    #define K1 1
#elif HFEn==312
    #define K3 14
    #define K2 7
    #define K1 1
#elif HFEn==313
    #define K3 7
    #define K2 3
    #define K1 1
#elif HFEn==314
    #define K3 15
#elif HFEn==315
    #define K3 10
    #define K2 9
    #define K1 1
#elif HFEn==316
    #define K3 10
    #define K2 6
    #define K1 1
#elif HFEn==317
    #define K3 7
    #define K2 4
    #define K1 2
#elif HFEn==318
    #define K3 12
    #define K2 7
    #define K1 1
#elif HFEn==319
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==320
    #define K3 4
    #define K2 3
    #define K1 1
#elif HFEn==321
    #define K3 31
#elif HFEn==322
    #define K3 17
    #define K2 2
    #define K1 1
#elif HFEn==323
    #define K3 10
    #define K2 3
    #define K1 1
#elif HFEn==324
    #define K3 4
    #define K2 2
    #define K1 1
#elif HFEn==325
    #define K3 15
    #define K2 4
    #define K1 1
#elif HFEn==326
    #define K3 10
    #define K2 3
    #define K1 1
#elif HFEn==327
    #define K3 18
    #define K2 6
    #define K1 1
#elif HFEn==328
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==329
    #define K3 12
    #define K2 10
    #define K1 1
#elif HFEn==330
    #define K3 12
    #define K2 9
    #define K1 1
#elif HFEn==331
    #define K3 16
    #define K2 9
    #define K1 1
#elif HFEn==332
    #define K3 6
    #define K2 2
    #define K1 1
#elif HFEn==333
    #define K3 2
#elif HFEn==334
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==335
    #define K3 14
    #define K2 4
    #define K1 1
#elif HFEn==336
    #define K3 7
    #define K2 4
    #define K1 1
#elif HFEn==337
    #define K3 10
    #define K2 6
    #define K1 1
#elif HFEn==338
    #define K3 4
    #define K2 3
    #define K1 1
#elif HFEn==339
    #define K3 23
    #define K2 14
    #define K1 1
#elif HFEn==340
    #define K3 14
    #define K2 6
    #define K1 1
#elif HFEn==341
    #define K3 24
    #define K2 23
    #define K1 1
#elif HFEn==342
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==343
    #define K3 15
    #define K2 10
    #define K1 1
#elif HFEn==344
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==345
    #define K3 22
#elif HFEn==346
    #define K3 21
    #define K2 10
    #define K1 1
#elif HFEn==347
    #define K3 14
    #define K2 12
    #define K1 1
#elif HFEn==348
    #define K3 18
    #define K2 10
    #define K1 1
#elif HFEn==349
    #define K3 12
    #define K2 11
    #define K1 1
#elif HFEn==350
    #define K3 11
    #define K2 4
    #define K1 1
#elif HFEn==351
    #define K3 11
    #define K2 7
    #define K1 1
#elif HFEn==352
    #define K3 13
    #define K2 11
    #define K1 6
#elif HFEn==353
    #define K3 20
    #define K2 9
    #define K1 1
#elif HFEn==354
    #define K3 16
    #define K2 15
    #define K1 1
#elif HFEn==355
    #define K3 6
    #define K2 5
    #define K1 1
#elif HFEn==356
    #define K3 12
    #define K2 4
    #define K1 1
#elif HFEn==357
    #define K3 28
    #define K2 2
    #define K1 1
#elif HFEn==358
    #define K3 24
    #define K2 4
    #define K1 1
#elif HFEn==359
    #define K3 9
    #define K2 7
    #define K1 1
#elif HFEn==360
    #define K3 13
    #define K2 11
    #define K1 1
#elif HFEn==361
    #define K3 7
    #define K2 4
    #define K1 1
#elif HFEn==362
    #define K3 13
    #define K2 3
    #define K1 1
#elif HFEn==363
    #define K3 8
    #define K2 7
    #define K1 1
#elif HFEn==364
    #define K3 9
#elif HFEn==365
    #define K3 18
    #define K2 14
    #define K1 1
#elif HFEn==366
    #define K3 29
#elif HFEn==367
    #define K3 21
#elif HFEn==368
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==369
    #define K3 14
    #define K2 4
    #define K1 1
#elif HFEn==370
    #define K3 9
    #define K2 2
    #define K1 1
#elif HFEn==371
    #define K3 16
    #define K2 15
    #define K1 1
#elif HFEn==372
    #define K3 17
    #define K2 5
    #define K1 1
#elif HFEn==373
    #define K3 9
    #define K2 6
    #define K1 1
#elif HFEn==374
    #define K3 16
    #define K2 14
    #define K1 1
#elif HFEn==375
    #define K3 16
#elif HFEn==376
    #define K3 15
    #define K2 9
    #define K1 1
#elif HFEn==377
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==378
    #define K3 17
    #define K2 13
    #define K1 1
#elif HFEn==379
    #define K3 17
    #define K2 6
    #define K1 1
#elif HFEn==380
    #define K3 17
    #define K2 12
    #define K1 1
#elif HFEn==381
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==382
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==383
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==384
    #define K3 27
    #define K2 6
    #define K1 1
#elif HFEn==385
    #define K3 6
#elif HFEn==386
    #define K3 16
    #define K2 2
    #define K1 1
#elif HFEn==387
    #define K3 8
    #define K2 7
    #define K1 1
#elif HFEn==388
    #define K3 14
    #define K2 3
    #define K1 1
#elif HFEn==389
    #define K3 17
    #define K2 10
    #define K1 1
#elif HFEn==390
    #define K3 9
#elif HFEn==391
    #define K3 28
#elif HFEn==392
    #define K3 15
    #define K2 14
    #define K1 1
#elif HFEn==393
    #define K3 7
#elif HFEn==394
    #define K3 15
    #define K2 7
    #define K1 1
#elif HFEn==395
    #define K3 22
    #define K2 20
    #define K1 1
#elif HFEn==396
    #define K3 25
#elif HFEn==397
    #define K3 22
    #define K2 3
    #define K1 1
#elif HFEn==398
    #define K3 23
    #define K2 3
    #define K1 1
#elif HFEn==399
    #define K3 26
#elif HFEn==400
    #define K3 14
    #define K2 11
    #define K1 1
#elif HFEn==401
    #define K3 19
    #define K2 8
    #define K1 1
#elif HFEn==402
    #define K3 22
    #define K2 19
    #define K1 1
#elif HFEn==403
    #define K3 28
    #define K2 6
    #define K1 1
#elif HFEn==404
    #define K3 14
    #define K2 3
    #define K1 1
#elif HFEn==405
    #define K3 28
    #define K2 18
    #define K1 1
#elif HFEn==406
    #define K3 29
    #define K2 8
    #define K1 1
#elif HFEn==407
    #define K3 13
    #define K2 5
    #define K1 1
#elif HFEn==408
    #define K3 7
    #define K2 5
    #define K1 1
#elif HFEn==409
    #define K3 15
    #define K2 6
    #define K1 1
#elif HFEn==410
    #define K3 16
    #define K2 2
    #define K1 1
#elif HFEn==411
    #define K3 18
    #define K2 3
    #define K1 1
#elif HFEn==412
    #define K3 18
    #define K2 12
    #define K1 1
#elif HFEn==413
    #define K3 10
    #define K2 7
    #define K1 6
#elif HFEn==414
    #define K3 13
#elif HFEn==415
    #define K3 12
    #define K2 5
    #define K1 1
#elif HFEn==416
    #define K3 27
    #define K2 5
    #define K1 1
#elif HFEn==417
    #define K3 10
    #define K2 3
    #define K1 1
#elif HFEn==418
    #define K3 15
    #define K2 3
    #define K1 1
#elif HFEn==419
    #define K3 21
    #define K2 14
    #define K1 1
#elif HFEn==420
    #define K3 7
#elif HFEn==421
    #define K3 7
    #define K2 4
    #define K1 1
#elif HFEn==422
    #define K3 10
    #define K2 6
    #define K1 1
#elif HFEn==423
    #define K3 25
#elif HFEn==424
    #define K3 19
    #define K2 9
    #define K1 1
#elif HFEn==425
    #define K3 12
#elif HFEn==426
    #define K3 20
    #define K2 11
    #define K1 1
#elif HFEn==427
    #define K3 18
    #define K2 13
    #define K1 1
#elif HFEn==428
    #define K3 24
    #define K2 5
    #define K1 1
#elif HFEn==429
    #define K3 13
    #define K2 6
    #define K1 1
#elif HFEn==430
    #define K3 14
    #define K2 6
    #define K1 1
#elif HFEn==431
    #define K3 5
    #define K2 3
    #define K1 1
#elif HFEn==432
    #define K3 23
    #define K2 10
    #define K1 1
#elif HFEn==433
    #define K3 15
    #define K2 5
    #define K1 1
#elif HFEn==434
    #define K3 12
    #define K2 11
    #define K1 5
#elif HFEn==435
    #define K3 13
    #define K2 8
    #define K1 1
#elif HFEn==436
    #define K3 15
    #define K2 2
    #define K1 1
#elif HFEn==437
    #define K3 6
    #define K2 2
    #define K1 1
#elif HFEn==438
    #define K3 20
    #define K2 19
    #define K1 1
#elif HFEn==439
    #define K3 10
    #define K2 7
    #define K1 1
#elif HFEn==440
    #define K3 4
    #define K2 3
    #define K1 1
#elif HFEn==441
    #define K3 7
#elif HFEn==442
    #define K3 21
    #define K2 4
    #define K1 1
#elif HFEn==443
    #define K3 10
    #define K2 6
    #define K1 1
#elif HFEn==444
    #define K3 12
    #define K2 7
    #define K1 1
#elif HFEn==445
    #define K3 23
    #define K2 12
    #define K1 1
#elif HFEn==446
    #define K3 19
    #define K2 9
    #define K1 1
#elif HFEn==447
    #define K3 9
    #define K2 6
    #define K1 1
#elif HFEn==448
    #define K3 13
    #define K2 7
    #define K1 1
#elif HFEn==449
    #define K3 17
    #define K2 12
    #define K1 1
#elif HFEn==450
    #define K3 22
    #define K2 5
    #define K1 1
#elif HFEn==451
    #define K3 16
    #define K2 10
    #define K1 1
#elif HFEn==452
    #define K3 10
    #define K2 2
    #define K1 1
#elif HFEn==453
    #define K3 19
    #define K2 8
    #define K1 1
#elif HFEn==454
    #define K3 8
    #define K2 6
    #define K1 1
#elif HFEn==455
    #define K3 15
    #define K2 7
    #define K1 1
#elif HFEn==456
    #define K3 25
    #define K2 11
    #define K1 1
#elif HFEn==457
    #define K3 16
#elif HFEn==458
    #define K3 27
    #define K2 10
    #define K1 1
#elif HFEn==459
    #define K3 12
    #define K2 5
    #define K1 2
#elif HFEn==460
    #define K3 19
#elif HFEn==461
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==462
    #define K3 20
    #define K2 8
    #define K1 1
#elif HFEn==463
    #define K3 18
    #define K2 17
    #define K1 1
#elif HFEn==464
    #define K3 19
    #define K2 18
    #define K1 13
#elif HFEn==465
    #define K3 31
#elif HFEn==466
    #define K3 16
    #define K2 15
    #define K1 1
#elif HFEn==467
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==468
    #define K3 27
#elif HFEn==469
    #define K3 9
    #define K2 5
    #define K1 2
#elif HFEn==470
    #define K3 9
#elif HFEn==471
    #define K3 1
#elif HFEn==472
    #define K3 17
    #define K2 14
    #define K1 1
#elif HFEn==473
    #define K3 11
    #define K2 4
    #define K1 1
#elif HFEn==474
    #define K3 22
    #define K2 17
    #define K1 1
#elif HFEn==475
    #define K3 18
    #define K2 7
    #define K1 1
#elif HFEn==476
    #define K3 9
#elif HFEn==477
    #define K3 16
    #define K2 15
    #define K1 7
#elif HFEn==478
    #define K3 6
    #define K2 4
    #define K1 1
#elif HFEn==479
    #define K3 19
    #define K2 8
    #define K1 1
#elif HFEn==480
    #define K3 15
    #define K2 9
    #define K1 6
#elif HFEn==481
    #define K3 10
    #define K2 9
    #define K1 1
#elif HFEn==482
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==483
    #define K3 9
    #define K2 6
    #define K1 4
#elif HFEn==484
    #define K3 11
    #define K2 9
    #define K1 1
#elif HFEn==485
    #define K3 30
    #define K2 25
    #define K1 1
#elif HFEn==486
    #define K3 26
    #define K2 8
    #define K1 1
#elif HFEn==487
    #define K3 14
    #define K2 5
    #define K1 1
#elif HFEn==488
    #define K3 4
    #define K2 3
    #define K1 1
#elif HFEn==489
    #define K3 14
    #define K2 11
    #define K1 1
#elif HFEn==490
    #define K3 17
    #define K2 9
    #define K1 1
#elif HFEn==491
    #define K3 15
    #define K2 14
    #define K1 1
#elif HFEn==492
    #define K3 7
#elif HFEn==493
    #define K3 20
    #define K2 15
    #define K1 1
#elif HFEn==494
    #define K3 17
#elif HFEn==495
    #define K3 15
    #define K2 9
    #define K1 1
#elif HFEn==496
    #define K3 21
    #define K2 14
    #define K1 1
#elif HFEn==497
    #define K3 13
    #define K2 8
    #define K1 1
#elif HFEn==498
    #define K3 31
    #define K2 12
    #define K1 1
#elif HFEn==499
    #define K3 20
    #define K2 12
    #define K1 1
#elif HFEn==500
    #define K3 27
#elif HFEn==501
    #define K3 23
    #define K2 10
    #define K1 1
#elif HFEn==502
    #define K3 16
    #define K2 12
    #define K1 1
#elif HFEn==503
    #define K3 3
#elif HFEn==504
    #define K3 15
    #define K2 14
    #define K1 6
#elif HFEn==505
    #define K3 24
    #define K2 18
    #define K1 1
#elif HFEn==506
    #define K3 23
#elif HFEn==507
    #define K3 26
    #define K2 2
    #define K1 1
#elif HFEn==508
    #define K3 9
#elif HFEn==509
    #define K3 24
    #define K2 14
    #define K1 1
#elif HFEn==510
    #define K3 6
    #define K2 4
    #define K1 1
#elif HFEn==511
    #define K3 10
#elif HFEn==512
    #define K3 8
    #define K2 5
    #define K1 2
#elif HFEn==513
    #define K3 26
#elif HFEn==514
    #define K3 14
    #define K2 2
    #define K1 1
#elif HFEn==515
    #define K3 25
    #define K2 18
    #define K1 1
#elif HFEn==516
    #define K3 21
#elif HFEn==517
    #define K3 28
    #define K2 6
    #define K1 1
#elif HFEn==518
    #define K3 17
    #define K2 15
    #define K1 1
#elif HFEn==519
    #define K3 17
    #define K2 5
    #define K1 1
#elif HFEn==520
    #define K3 15
    #define K2 11
    #define K1 2
#elif HFEn==521
    #define K3 32
#elif HFEn==522
    #define K3 29
    #define K2 2
    #define K1 1
#elif HFEn==523
    #define K3 18
    #define K2 14
    #define K1 1
#elif HFEn==524
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==525
    #define K3 6
    #define K2 4
    #define K1 1
#elif HFEn==526
    #define K3 9
    #define K2 5
    #define K1 1
#elif HFEn==527
    #define K3 9
    #define K2 7
    #define K1 1
#elif HFEn==528
    #define K3 11
    #define K2 6
    #define K1 2
#elif HFEn==529
    #define K3 7
    #define K2 4
    #define K1 1
#elif HFEn==530
    #define K3 25
    #define K2 2
    #define K1 1
#elif HFEn==531
    #define K3 18
    #define K2 7
    #define K1 1
#elif HFEn==532
    #define K3 1
#elif HFEn==533
    #define K3 4
    #define K2 3
    #define K1 2
#elif HFEn==534
    #define K3 7
    #define K2 5
    #define K1 1
#elif HFEn==535
    #define K3 13
    #define K2 9
    #define K1 1
#elif HFEn==536
    #define K3 7
    #define K2 5
    #define K1 3
#elif HFEn==537
    #define K3 10
    #define K2 2
    #define K1 1
#elif HFEn==538
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==539
    #define K3 27
    #define K2 22
    #define K1 1
#elif HFEn==540
    #define K3 9
#elif HFEn==541
    #define K3 30
    #define K2 27
    #define K1 1
#elif HFEn==542
    #define K3 8
    #define K2 6
    #define K1 1
#elif HFEn==543
    #define K3 16
#elif HFEn==544
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==545
    #define K3 14
    #define K2 4
    #define K1 1
#elif HFEn==546
    #define K3 8
    #define K2 2
    #define K1 1
#elif HFEn==547
    #define K3 20
    #define K2 17
    #define K1 1
#elif HFEn==548
    #define K3 25
    #define K2 17
    #define K1 1
#elif HFEn==549
    #define K3 25
    #define K2 6
    #define K1 1
#elif HFEn==550
    #define K3 14
    #define K2 5
    #define K1 1
#elif HFEn==551
    #define K3 9
    #define K2 4
    #define K1 1
#elif HFEn==552
    #define K3 19
    #define K2 16
    #define K1 9
#elif HFEn==553
    #define K3 21
    #define K2 11
    #define K1 1
#elif HFEn==554
    #define K3 21
    #define K2 11
    #define K1 1
#elif HFEn==555
    #define K3 32
    #define K2 16
    #define K1 1
#elif HFEn==556
    #define K3 6
    #define K2 3
    #define K1 1
#elif HFEn==557
    #define K3 26
    #define K2 16
    #define K1 1
#elif HFEn==558
    #define K3 25
    #define K2 5
    #define K1 1
#elif HFEn==559
    #define K3 13
    #define K2 9
    #define K1 1
#elif HFEn==560
    #define K3 11
    #define K2 9
    #define K1 6
#elif HFEn==561
    #define K3 14
    #define K2 13
    #define K1 1
#elif HFEn==562
    #define K3 27
    #define K2 20
    #define K1 1
#elif HFEn==563
    #define K3 20
    #define K2 7
    #define K1 1
#elif HFEn==564
    #define K3 6
    #define K2 3
    #define K1 1
#elif HFEn==565
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==566
    #define K3 20
    #define K2 13
    #define K1 1
#elif HFEn==567
    #define K3 28
#elif HFEn==568
    #define K3 26
    #define K2 9
    #define K1 1
#elif HFEn==569
    #define K3 12
    #define K2 10
    #define K1 1
#elif HFEn==570
    #define K3 19
    #define K2 16
    #define K1 1
#elif HFEn==571
    #define K3 10
    #define K2 5
    #define K1 2
#elif HFEn==572
    #define K3 12
    #define K2 8
    #define K1 1
#elif HFEn==573
    #define K3 24
    #define K2 19
    #define K1 1
#elif HFEn==574
    #define K3 13
#elif HFEn==575
    #define K3 25
    #define K2 18
    #define K1 1
#elif HFEn==576
    #define K3 22
    #define K2 19
    #define K1 1
#elif HFEn==577
    #define K3 11
    #define K2 2
    #define K1 1
#elif HFEn==578
    #define K3 25
    #define K2 14
    #define K1 1
#elif HFEn==579
    #define K3 16
    #define K2 6
    #define K1 1
#elif HFEn==580
    #define K3 6
    #define K2 4
    #define K1 1
#elif HFEn==581
    #define K3 30
    #define K2 5
    #define K1 1
#elif HFEn==582
    #define K3 5
    #define K2 2
    #define K1 1
#elif HFEn==583
    #define K3 10
    #define K2 4
    #define K1 1
#elif HFEn==584
    #define K3 17
    #define K2 15
    #define K1 1
#elif HFEn==585
    #define K3 29
    #define K2 11
    #define K1 1
#elif HFEn==586
    #define K3 14
    #define K2 2
    #define K1 1
#elif HFEn==587
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==588
    #define K3 30
    #define K2 3
    #define K1 1
#elif HFEn==589
    #define K3 14
    #define K2 7
    #define K1 2
#elif HFEn==590
    #define K3 25
    #define K2 2
    #define K1 1
#elif HFEn==591
    #define K3 16
    #define K2 3
    #define K1 1
#elif HFEn==592
    #define K3 27
    #define K2 6
    #define K1 1
#elif HFEn==593
    #define K3 13
    #define K2 3
    #define K1 1
#elif HFEn==594
    #define K3 17
    #define K2 11
    #define K1 1
#elif HFEn==595
    #define K3 9
    #define K2 2
    #define K1 1
#elif HFEn==596
    #define K3 28
    #define K2 2
    #define K1 1
#elif HFEn==597
    #define K3 25
    #define K2 8
    #define K1 2
#elif HFEn==598
    #define K3 7
    #define K2 6
    #define K1 1
#elif HFEn==599
    #define K3 28
    #define K2 6
    #define K1 1
#elif HFEn==600
    #define K3 11
    #define K2 10
    #define K1 1
#elif HFEn==601
    #define K3 12
    #define K2 4
    #define K1 1
#elif HFEn==602
    #define K3 29
    #define K2 4
    #define K1 1
#elif HFEn==603
    #define K3 8
    #define K2 4
    #define K1 1
#elif HFEn==604
    #define K3 25
    #define K2 6
    #define K1 1
#elif HFEn==605
    #define K3 11
    #define K2 6
    #define K1 1
#elif HFEn==606
    #define K3 22
    #define K2 3
    #define K1 1
#elif HFEn==607
    #define K3 15
    #define K2 3
    #define K1 1
#elif HFEn==608
    #define K3 31
    #define K2 3
    #define K1 1
#elif HFEn==609
    #define K3 16
    #define K2 5
    #define K1 1
#elif HFEn==610
    #define K3 29
    #define K2 27
    #define K1 1
#elif HFEn==611
    #define K3 13
    #define K2 2
    #define K1 1
#elif HFEn==612
    #define K3 11
    #define K2 10
    #define K1 1
#elif HFEn==613
    #define K3 31
    #define K2 10
    #define K1 1
#elif HFEn==614
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==615
    #define K3 11
    #define K2 5
    #define K1 1
#elif HFEn==616
    #define K3 22
    #define K2 9
    #define K1 2
#elif HFEn==617
    #define K3 11
    #define K2 3
    #define K1 1
#elif HFEn==618
    #define K3 21
    #define K2 15
    #define K1 1
#elif HFEn==619
    #define K3 21
    #define K2 12
    #define K1 1
#elif HFEn==620
    #define K3 9
    #define K2 2
    #define K1 1
#elif HFEn==621
    #define K3 17
    #define K2 2
    #define K1 1
#elif HFEn==622
    #define K3 16
    #define K2 10
    #define K1 1
#elif HFEn==623
    #define K3 31
    #define K2 3
    #define K1 1
#elif HFEn==624
    #define K3 16
    #define K2 15
    #define K1 1
#elif HFEn==625
    #define K3 19
    #define K2 4
    #define K1 1
#elif HFEn==626
    #define K3 30
    #define K2 13
    #define K1 2
#elif HFEn==627
    #define K3 32
    #define K2 7
    #define K1 1
#elif HFEn==628
    #define K3 26
    #define K2 11
    #define K1 1
#elif HFEn==629
    #define K3 8
    #define K2 3
    #define K1 1
#elif HFEn==630
    #define K3 30
    #define K2 9
    #define K1 1
#elif HFEn==631
    #define K3 31
    #define K2 13
    #define K1 1
#elif HFEn==632
    #define K3 9
    #define K2 2
    #define K1 1
#elif HFEn==633
    #define K3 7
    #define K2 2
    #define K1 1
#elif HFEn==634
    #define K3 24
    #define K2 2
    #define K1 1
#elif HFEn==635
    #define K3 26
    #define K2 16
    #define K1 1
#elif HFEn==636
    #define K3 25
    #define K2 4
    #define K1 1
#elif HFEn==637
    #define K3 27
    #define K2 8
    #define K1 1
#elif HFEn==638
    #define K3 14
    #define K2 4
    #define K1 1
#elif HFEn==639
    #define K3 31
    #define K2 3
    #define K1 1
#elif HFEn==640
    #define K3 28
    #define K2 27
    #define K1 1
#else
    #error "config_gf2n.h has no irreducible polynomial for this HFEn"
#endif


#ifdef K2
    /* Pentanomial */
    #define __PENTANOMHFE__
#else
    /* Trinomial */
    #define __TRINOMHFE__
#endif

#ifdef __PENTANOMHFE__
    #if ((K1==0)||(K1>=K2)||(K2>=K3))
        #error "0 < K1 < K2 < K3 is required"
    #endif
#endif

#if (K3>32)
    #error "The modular reduction requires K3<=32"
#endif

#endif
