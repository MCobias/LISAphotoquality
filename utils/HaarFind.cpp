//
//  imagetester.cpp
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "HaarFind.hpp"

/**
* @brief Load Cascade Classifier
*
* @details Carrega o classificador para identificacao das faces
*
*/
void HaarFind::loadCascadeClassifier()
{
  if (!this->faceCascade.load("./data/haarcascade_frontalface_alt.xml"))
  {
    printf("(!) Error Loading Face Cascade Classifier\n");
  }
  if (!this->eyePairCascade.load("./data/haarcascade_eye_pair_45x11_parojosG.xml"))
  {
    printf("(!) Error Loading Eye Pair Cascade Classifier\n");
  }
}

HaarFind::HaarFind(Mat image)
{
  loadCascadeClassifier();
  Mat imageGray = Util::coloredToGray(image);
  this->face = this->findFace(imageGray);
  Mat imgFace = imageGray(face);
  Rect_<double> eyes = this->findEyesPair(imgFace);
  this->eyePair = eyes;
  calculeEyeSingle(imgFace, eyes);
}

Rect_<double> HaarFind::findFace(Mat image)
{
  vector<Rect> faces;
  Rect_<double> singleFace;
  vector<Rect_<double>> regionsFaces;
  faceCascade.detectMultiScale(image, faces, 1.2, 2, 0, Size(50, 50));

  if(faces.size() > 0)
	{
    regionsFaces.resize(faces.size());
    for(size_t i = 0; i < regionsFaces.size(); i++)
	  {
      regionsFaces[i].width = faces[i].width * 0.8924;
		  regionsFaces[i].height = faces[i].height * 0.8676;
		  regionsFaces[i].x = faces[i].x + 0.0578 * faces[i].width;
		  regionsFaces[i].y = faces[i].y + faces[i].height * 0.2166;
    }

    if(regionsFaces.size() > 1)
		{
			double best = -1;
			int bestIndex = -1;
			for(size_t i = 0; i < regionsFaces.size(); i++)
			{
				double dist = regionsFaces[i].width;
				bool better = regionsFaces[i].width > best;

        // Pick a closest face to preffered point or the biggest face
				if(i == 0 || better)
				{
					bestIndex = i;
					best = dist;
				}
			}
			singleFace = regionsFaces[bestIndex];
		}
		else
		{
		 return regionsFaces[0];
    }
 }
  else
  {
    return Rect_<double>(0,0,0,0);
  }
  return singleFace;
}

Rect_<double> HaarFind::findEyesPair(Mat face)
{
    vector<Rect> eyes;
    vector<Rect_<double>> eyePair;
    equalizeHist(face, face);

    Rect_<double> eyePairRegion;
    this->eyePairCascade.detectMultiScale(face, eyes, 1.01, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(100, 30));

    if (eyes.size() == 0)
    {
        cout << "@WARNING: No Eye Pair found on file ." << endl;
    }
    else
     {
       eyePair.resize(eyes.size());
       for(size_t i = 0; i < eyes.size(); i++)
       {
          eyePair[i] = eyes[i];
       }
       eyePairRegion = eyePair[0];
        if (eyePair.size() > 1)
        {
            for (int i = 0; i < eyePair.size(); i++)
            {
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

                if (percent >= 74 || percent <= 56)
                {
                    continue;
                }
                // Calcular a porcentagem posicao
                int mainHgt = face.rows;
                int posEyeP = eyePair[i].y;

                //get % size of eye pair region
                int percentPos = (int)(posEyeP * 100) / mainHgt;
               // cout << "% pos: " << percentPos << endl;

                if (percentPos >= 34 || percentPos <= 20)
                {
                    continue;
                }

               // cout << "Y: " << eyePair[i].y << endl;
                if (eyePair[i].y >= 120 || eyePair[i].y <= 65)
                {
                    continue;
                }
               // cout << "Y REG: " << eyePairRegion.y << endl;
                if (eyePair[i].width > eyePairRegion.width)
                {
                    eyePairRegion = eyePair[i];
                }
            }
        }
    }
    return eyePairRegion;
}

void HaarFind::calculeEyeSingle(Mat imageFace, Rect eyePair)
{
  Mat imgRegion = Util::cutImage(imageFace, eyePair);
  double midle = imgRegion.cols / 2;
  this->rectEyeLeft = Rect_<double>(0, 0, midle, imgRegion.rows);
  this->rectEyeRight = Rect_<double>(midle, 0, midle, imgRegion.rows);
}

Rect_<double> HaarFind::getRoiFace()
{
    return this->face;
}

Rect_<double> HaarFind::getRoiEyersPair()
{
    return this->eyePair;
}

Rect_<double> HaarFind::getRoiEyeLeft()
{
    return this->rectEyeLeft;
}

Rect_<double> HaarFind::getRoiEyeRight()
{
    return this->rectEyeRight;
}

HaarFind::~HaarFind(){}
