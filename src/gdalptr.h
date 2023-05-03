
#include "gdal_priv.h"

#include <R.h>
#include <Rinternals.h>

template <typename T>
static inline const char* gdalptr_xptr_class();

template <>
inline const char* gdalptr_xptr_class<GDALDataset>() {
  return "gdal_dataset";
}

template <>
inline const char* gdalptr_xptr_class<GDALDriver>() {
  return "gdal_driver";
}


template <typename T>
static inline T* gdalptr_from_xptr(SEXP xptr) {
  if (!Rf_inherits(xptr, gdalptr_xptr_class<T>())) {
    Rf_error("Expected external pointer with class '%s'", gdalptr_xptr_class<T>());
  }

  T* ptr = reinterpret_cast<T*>(R_ExternalPtrAddr(xptr));
  if (ptr == nullptr) {
    Rf_error("Can't convert external pointer to NULL to T*");
  }
  return ptr;
}



template <typename T>
static inline SEXP gdalptr_allocate_xptr(SEXP shelter_sexp = R_NilValue) {
  void* ptr = malloc(sizeof(T));
  if (ptr == nullptr) {
    Rf_error("Failed to allocate T");
  }

  memset(ptr, 0, sizeof(T));
  SEXP xptr = PROTECT(R_MakeExternalPtr((void *)ptr, R_NilValue, shelter_sexp));
  SEXP xptr_class = PROTECT(Rf_allocVector(STRSXP, 2));
  SET_STRING_ELT(xptr_class, 0, Rf_mkChar(gdalptr_xptr_class<T>()));
  SET_STRING_ELT(xptr_class, 1, Rf_mkChar("gdalptr_xptr"));
  Rf_setAttrib(xptr, R_ClassSymbol, xptr_class);
  UNPROTECT(1);

  SEXP new_env_call = PROTECT(Rf_lang1(Rf_install("new_env")));
  SEXP new_env = PROTECT(Rf_eval(new_env_call, R_FindNamespace(Rf_mkString("gdalptr"))));
  R_SetExternalPtrTag(xptr, new_env);
  UNPROTECT(2);

  UNPROTECT(1);
  return xptr;
}

template <typename T>
static inline void gdalptr_xptr_default_finalize(SEXP xptr) {
  Rprintf("gdalptr dataset finalizer called to free the C pointer memory\n"); 
  T* ptr = reinterpret_cast<T*>(R_ExternalPtrAddr(xptr));
  if (ptr != nullptr) {
    free(ptr);
  }
  R_ClearExternalPtr(xptr); 
}

static inline const char* gdalptr_as_const_char(SEXP sexp) {
  if (TYPEOF(sexp) != STRSXP || Rf_length(sexp) != 1) {
    Rf_error("Expected character(1) for conversion to const char*");
  }

  SEXP item = STRING_ELT(sexp, 0);
  if (item == NA_STRING) {
    Rf_error("Can't convert NA_character_ to const char*");
  }

  return Rf_translateCharUTF8(item);
}


static inline int gdalptr_as_int(SEXP sexp) {
  if (Rf_length(sexp) == 1) {
    switch (TYPEOF(sexp)) {
    case REALSXP:
      return REAL(sexp)[0];
    case INTSXP:
      return INTEGER(sexp)[0];
    }
  }
  
  Rf_error("Expected integer(1) or double(1) for conversion to int");
}
// 
// static inline SEXP gdalptr_wrap_status(AdbcStatusCode code) {
//   return Rf_ScalarInteger(code);
// }

