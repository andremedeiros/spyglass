#ifndef SPYGLASS_PRELUDE_H_
#define SPYGLASS_PRELUDE_H_

#ifndef Data_Set_Struct
#define Data_Set_Struct(obj,ptr) do {\
  Check_Type(obj, T_DATA); \
  DATA_PTR(obj) = ptr; \
} while (0);
#endif

#define SG_GET(obj, type, var) \
  type *var; \
  Data_Get_Struct(obj, type, var);

#define SG_GET_CLASSIFIER(obj, var)     SG_GET(obj, cv::CascadeClassifier, var)
#define SG_GET_IMAGE(obj, var)          SG_GET(obj, cv::Mat, var)
#define SG_GET_POINT(obj, var)          SG_GET(obj, cv::Point, var)
#define SG_GET_RECT(obj, var)           SG_GET(obj, cv::Rect, var)
#define SG_GET_SIZE(obj, var)           SG_GET(obj, cv::Size, var)
#define SG_GET_VIDEO_CAPTURE(obj, var)  SG_GET(obj, cv::VideoCapture, var)

#endif // SPYGLASS_PRELUDE_H_
