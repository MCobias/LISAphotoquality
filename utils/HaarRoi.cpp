/**
*    Copyright (C) 2016 Lucas Porto, Flávio Vidal and Díbio Borges.
*/

#include "HaarRoi.hpp"

HaarRoi::HaarRoi(Mat image){
    
    loadCascadeClassifier();
    
    Rect face = this->findFace(image);
    Mat imgFace = image(face);
    Rect eyes = this->findEyesPair(imgFace);
    this->eyePair = eyes;

    eyes.x += face.x;
    eyes.y += face.y;

    double percent = (eyes.width * 20) / 100;
    
    this->customROI = Rect(eyes);
  
    if ((this->customROI.x - percent) > 0)  this->customROI.x -= percent;
    if ((this->customROI.y - percent) > 0)  this->customROI.y -= percent;

    this->customROI.width += (2 * percent);
    this->customROI.height *= 6.2;
}

Rect HaarRoi::getCunstomRoi() {
    return this->customROI;
}

/**
* @brief Load Cascade Classifier
*
* @details Carrega o classificador para identificacao das faces
*
*/
void HaarRoi::loadCascadeClassifier() {
    if (!this->face_cascade.load("haar/haarcascade_frontalface_alt.xml")) {
        printf("(!) Error Loading Face Cascade Classifier\n");
    }
    if (!this->eye_pair_cascade.load("haar/haarcascade_eye_pair_45x11_parojosG.xml")) {
        printf("(!) Error Loading Eye Pair Cascade Classifier\n");
    }
}

Rect HaarRoi::findFace(Mat image) {
    vector<Rect> faces;
    faces.clear();
    face_cascade.detectMultiScale(image, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(150, 100));

    Rect faceAux = faces[0];
    for (int i = 0; i < faces.size(); i++) {
        if (this->faceIsCentralized(Rect(0, 0, image.cols, image.rows), faces[i]) == false) {
            cout << "@WARNING: Face found but it is not centralized..." << endl;
            continue;
        }
        faceAux = faces[i];
    }
    return faceAux;
}

Rect HaarRoi::findEyesPair(Mat face) {
    vector<Rect> eyePair;

    equalizeHist(face, face);

    Rect eyePairRegion;

    eyePair.clear();
    this->eye_pair_cascade.detectMultiScale(face, eyePair, 1.01, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 30));

    if (eyePair.size() <= 0) {
        cout << "@WARNING: No Eye Pair found on file ." << endl;
    }
    else {
        eyePairRegion = eyePair[0];
        if (eyePair.size() > 1) {
            for (int i = 0; i < eyePair.size(); i++) {

                //rectangle(face, eyePair[i], Scalar(0, 255, 0));
                //imwrite("img_faces/" + imgNome, face);

                //********************************
                // Calcular a porcentagem do tamanho largura do par olhos
                int mainWdt = face.cols;
                int eyeWdt = eyePair[i].width;

                Mat eyeAux = face(eyePair[i]);

                //get % size of eye pair region
                int percent = (int)(eyeWdt * 100) / mainWdt;
                //cout << "% size: " << percent << endl;

                if (percent >= 74 || percent <= 56) {
                    continue;
                }

                // Calcular a porcentagem posicao 
                int mainHgt = face.rows;
                int posEyeP = eyePair[i].y;

                //get % size of eye pair region
                int percentPos = (int)(posEyeP * 100) / mainHgt;
               // cout << "% pos: " << percentPos << endl;

                if (percentPos >= 34 || percentPos <= 20) {
                    continue;
                }

               // cout << "Y: " << eyePair[i].y << endl;
                if (eyePair[i].y >= 120 || eyePair[i].y <= 65) {
                    continue;
                }
               // cout << "Y REG: " << eyePairRegion.y << endl;
                if (eyePair[i].width > eyePairRegion.width) {
                    eyePairRegion = eyePair[i];

                }
            }
        }
    }
    return eyePairRegion;
}


/**
* @brief Verify is face is centralizes
*
* @details Verifica se a face esta centralizada
*
*/
bool HaarRoi::faceIsCentralized(Rect mainImg, Rect face) {
    bool res = false;

    int allow = (int)mainImg.width * 0.10;
    int centerMainImgY = (int)mainImg.height / 2;
    int centerMainImgX = (int)mainImg.width / 2;

    int minX = centerMainImgX - allow;
    int maxX = centerMainImgX + allow;
    int minY = centerMainImgY - allow;
    int maxY = centerMainImgY + allow;

    int centerFaceX = face.x + ((int)face.width / 2);
    int centerFaceY = face.y + ((int)face.height / 2);

    if (centerFaceX >= minX && centerFaceX <= maxX &&
        centerFaceY >= minY && centerFaceY <= maxY) res = true;

    return res;
}

HaarRoi::~HaarRoi(){
}
