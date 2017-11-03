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
    missFaceLandmark = 11,
    missFacePose = 12,
    missDimension = 13,
    missBlur = 14,
    missBrightness = 15,
    missContrast = 16,
    missProportion = 17,
    missAlignment = 18,
    missEyeOpen = 19,
    missEyeGlasses = 20,
    missEyeLookAway = 21,
    missMouthBeardObstruction = 22,
    missMouthOpen = 23
};
#endif
