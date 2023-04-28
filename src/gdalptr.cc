#include "gdal_priv.h"
#include "gdalptr.h"
#include <R.h>
#include <Rinternals.h>


static void finalize_dataset_xptr(SEXP dataset_xptr) {
  auto* dataset = reinterpret_cast<GDALDataset*>(R_ExternalPtrAddr(dataset_xptr));
  if (dataset == nullptr) {
    return;
  }

  gdalptr_xptr_default_finalize<GDALDataset>(dataset_xptr);
}

// dataset
extern "C" SEXP GdalPtrDataset() {
  SEXP dataset_xptr = PROTECT(gdalptr_allocate_xptr<GDALDataset>());
  R_RegisterCFinalizer(dataset_xptr, &finalize_dataset_xptr);
  auto* dataset = gdalptr_from_xptr<GDALDataset>(dataset_xptr);
  
  UNPROTECT(1);
  return dataset_xptr;
}

extern "C" SEXP GdalPtrOpen(SEXP dataset_xptr, SEXP dsn_name_sexp) {
  GDALAllRegister();
  const char* dsn_name = gdalptr_as_const_char(dsn_name_sexp);      
  auto dataset = gdalptr_from_xptr<GDALDataset>(dataset_xptr);
  dataset =  (GDALDataset *)GDALOpen(dsn_name, GA_ReadOnly);

  
  R_SetExternalPtrAddr(dataset_xptr, dataset); 
  //Rprintf("Raster count: %i\n", (int) dataset->GetRasterCount()); 
  return dataset_xptr;
}


extern "C" SEXP GdalPtrGetRasterSize(SEXP dataset_xptr) {
  GDALAllRegister();
  
  auto dataset = gdalptr_from_xptr<GDALDataset>(dataset_xptr);
  
  SEXP result = PROTECT(Rf_allocVector(INTSXP, 2));
  
  INTEGER(result)[0] = (int) dataset->GetRasterXSize(); 
  INTEGER(result)[1] = (int) dataset->GetRasterYSize();
  UNPROTECT(1); 
  return result;
}

extern "C" SEXP GdalVSIReadDirRecursive(SEXP dsn_name_sexp) {
  const char* dsn_name = gdalptr_as_const_char(dsn_name_sexp);      
   
  char** VSI_paths  = VSIReadDirRecursive(dsn_name);
  
  int ipath = 0; // iterate to count
  while (VSI_paths && VSI_paths[ipath] != NULL) {
    ipath++;
  }
  SEXP result = PROTECT(Rf_allocVector(STRSXP, ipath));
  
  for (int i = 0; i < ipath; i++) {
    SET_STRING_ELT(result, i, Rf_mkChar(VSI_paths[i]));
  }
  
  CSLDestroy(VSI_paths);
  UNPROTECT(1); 
  return result;
}



// standalone
extern "C" SEXP GdalVersion() {

  SEXP result = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(result, 0, Rf_mkChar(GDALVersionInfo("--version")));
  UNPROTECT(1); 
  return result;
}


