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
#include "bgslib_dp_prati_mediod_bgs.h"

DPPratiMediodBGS::DPPratiMediodBGS() :
  firstTime(true), frameNumber(0), threshold(30), samplingRate(5), historySize(16), weight(5), showOutput(false)
{}

DPPratiMediodBGS::~DPPratiMediodBGS() {}

void DPPratiMediodBGS::operator()(cv::InputArray _image, cv::OutputArray _fgmask, double learningRate)
{
  cv::Mat img_input = _image.getMat();
  if(img_input.empty())
    return;

  frame = new IplImage(img_input);

  if(firstTime)
    frame_data.ReleaseMemory(false);
  frame_data = frame;

  if(firstTime)
  {
    int width	= img_input.size().width;
    int height = img_input.size().height;

    lowThresholdMask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    lowThresholdMask.Ptr()->origin = IPL_ORIGIN_BL;

    highThresholdMask = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    highThresholdMask.Ptr()->origin = IPL_ORIGIN_BL;

    params.SetFrameSize(width, height);
    params.LowThreshold() = threshold;
    params.HighThreshold() = 2*params.LowThreshold();	// Note: high threshold is used by post-processing
    params.SamplingRate() = samplingRate;
    params.HistorySize() = historySize;
    params.Weight() = weight;

    bgs.Initalize(params);
    bgs.InitModel(frame_data);
  }

  bgs.Subtract(frameNumber, frame_data, lowThresholdMask, highThresholdMask);
  lowThresholdMask.Clear();
  bgs.Update(frameNumber, frame_data, lowThresholdMask);

  cv::Mat foreground(highThresholdMask.Ptr());

  if(showOutput)
    cv::imshow("Temporal Median (Cucchiara&Calderara)", foreground);

  foreground.copyTo(_fgmask);

  delete frame;
  firstTime = false;
  frameNumber++;
}

CV_INIT_ALGORITHM(DPPratiMediodBGS, "BGSLibrary.DPPratiMediod",
    obj.info()->addParam(obj, "threshold",    obj.threshold);
    obj.info()->addParam(obj, "samplingRate", obj.samplingRate);
    obj.info()->addParam(obj, "historySize",  obj.historySize);
    obj.info()->addParam(obj, "showOutput",   obj.showOutput));
