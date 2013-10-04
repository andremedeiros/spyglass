/*
This file is part of BGSLibrary.

BGSLibrary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BGSLibrary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BGSLibrary.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/internal.hpp>

#include "bgslib_prati_mediod_bgs.h"

using namespace Algorithms::BackgroundSubtraction;

class DPPratiMediodBGS : public cv::BackgroundSubtractor
{
private:
  bool firstTime;
  long frameNumber;
  IplImage* frame;
  RgbImage frame_data;

  PratiParams params;
  PratiMediodBGS bgs;
  BwImage lowThresholdMask;
  BwImage highThresholdMask;

  int threshold;
  int samplingRate;
  int historySize;
  int weight;
  bool showOutput;

public:
  DPPratiMediodBGS();
  ~DPPratiMediodBGS();

  virtual cv::AlgorithmInfo* info() const;
  void operator()(cv::InputArray _image, cv::OutputArray _fgmask, double learningRate);
};

