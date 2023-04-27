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
  
  //Rprintf("Raster count: %i\n", (int) dataset->GetRasterCount()); 
  return dataset_xptr;
}


extern "C" SEXP GdalPtrGetInfo(SEXP dataset_xptr) {
  GDALAllRegister();
  
  auto dataset = gdalptr_from_xptr<GDALDataset>(dataset_xptr);
  
  if (dataset == nullptr) {
    Rprintf("it null!\n"); 
  }
  int nc = (int) dataset->GetRasterXSize();
  int nr = (int) dataset->GetRasterYSize();
  
  //Rprintf("Raster version info: %s\n", GDALVersionInfo("--version")); 
  return Rf_ScalarInteger(nc);
}

