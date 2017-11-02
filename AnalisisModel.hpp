//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#ifndef AnalysisModel_HPP
#define AnalysisModel_HPP

enum analysisCode
{
    notAnalyzed = -1,
    inadequate = 0,
    adequate = 1,
    missFaceHaar = 10,
    missFacePose = 11,
    missDimension = 12,
    missBlur = 13,
    missBrightness = 14,
    missContrast = 15,
    missVariance = 16,
    missProportion = 17,
    missAlignment = 18,
    missEyeOpen = 19,
    missEyeGlasses = 20,
    missEyeLookAway = 21,
    missMouthBeardObstruction = 22,
    missMouthOpen = 23
};
#endif
