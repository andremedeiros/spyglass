#ifndef SPYGLASS_PRELUDE_H_
#define SPYGLASS_PRELUDE_H_

#ifndef Data_Set_Struct
#define Data_Set_Struct(obj,ptr) do {\
  Check_Type(obj, T_DATA); \
  DATA_PTR(obj) = ptr; \
} while (0);
#endif

#endif // SPYGLASS_PRELUDE_H_
