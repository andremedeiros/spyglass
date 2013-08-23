#ifndef ROPENCV_PRELUDE_H_
#define ROPENCV_PRELUDE_H_

#ifndef Data_Set_Struct
#define Data_Set_Struct(obj,ptr) do {\
  Check_Type(obj, T_DATA); \
  DATA_PTR(obj) = ptr; \
} while (0);
#endif

#endif
