20x20 20x20:
-template: 57.88262

-Log:
    38.03994
    0.0088789463
    0.005448102951050 default static, 1
    0.015083789825439 dynamic, 2
    0.004941940307617 dynamic, 5
    0.003712177276611 dynamic, 10
    0.003430843353271 dynamic, 20 with 400 at freeing memory
    0.003931999206543 dynamic, 20 with parallel reading matrix
    0.009217977523804 no schedule but major changes to isMagicSquare and isPairwiseDistinct
    0.236486911773682 with target teams loop
    0.007000207901001 with optimised generateMagicSquare
    0.009648799896240 if is magic matrix, 0.001686096191406 if not
    49.149247884750366 simple one
    0.083806991577148 simple
    0.081314086914062 non cache blocking
    0.081989049911499 ^
    0.079741954803467 ^
    0.083863019943237 ^
    0.068506956100464 cache blocking
    0.070659875869751 ^
    0.069304943084717 ^
    0.068883895874023 ^
    0.073215007781982
    0.071928024291992

    


3x3 3x3:
-template: 0.00003

-Log:
    0.000058889389038
    0.000077009201050 dynamic, 20 with 400 at freeing memory
    0.000037908554077 (best: 0.000013113021851) no schedule but major changes to isMagicSquare.etc.
    0.000112056732178 simple file


10x10 10x10:
-template: 0.24376

-Log:
    0.162724971771240
    0.000325918197632 dynamic, 20 with 400 at freeing memory
    