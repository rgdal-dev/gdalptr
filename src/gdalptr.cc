#include "gdal_priv.h"
#include "gdalptr.h"
#include <R.h>
#include <Rinternals.h>

inline GDALRasterIOExtraArg init_resample_alg(SEXP resample_algo) {
  const char* resample = gdalptr_as_const_char(resample_algo);      
  
  GDALRasterIOExtraArg psExtraArg;
  INIT_RASTERIO_EXTRA_ARG(psExtraArg);
  if (EQUAL(resample,  "average")) {
    psExtraArg.eResampleAlg = GRIORA_Average;
  }
  if (EQUAL(resample,  "bilinear")) {
    psExtraArg.eResampleAlg = GRIORA_Bilinear;
  }
  if (EQUAL(resample,  "cubic")) {
    psExtraArg.eResampleAlg = GRIORA_Cubic;
  }
  if (EQUAL(resample,  "cubicspline")) {
    psExtraArg.eResampleAlg = GRIORA_CubicSpline;
  }
  if (EQUAL(resample,  "gauss")) {
    psExtraArg.eResampleAlg = GRIORA_Gauss;
  }
  if (EQUAL(resample,  "lanczos")) {
    psExtraArg.eResampleAlg = GRIORA_Lanczos;
  }
  if (EQUAL(resample,  "mode")) {
    psExtraArg.eResampleAlg = GRIORA_Mode;
  }
  if (EQUAL(resample,  "nearestneighbour")) {
    psExtraArg.eResampleAlg = GRIORA_NearestNeighbour;
  }
  return psExtraArg;
}


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

extern "C" SEXP GdalPtrGDALOpen(SEXP dataset_xptr, SEXP dsn_name_sexp, SEXP read_only) {
  GDALAllRegister();
  const char* dsn_name = gdalptr_as_const_char(dsn_name_sexp);      
  auto dataset = gdalptr_from_xptr<GDALDataset>(dataset_xptr);
  bool write = asLogical(read_only); 
  if (write) {
    dataset =  (GDALDataset *)GDALOpen(dsn_name, GA_Update);
  } else {
    
    dataset =  (GDALDataset *)GDALOpen(dsn_name, GA_ReadOnly);
  }
  
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


extern "C" SEXP GdalPtrRasterIO(SEXP dataset_xptr, SEXP window, SEXP data, SEXP resample_algo) {
  GDALAllRegister();
  
  auto dataset = gdalptr_from_xptr<GDALDataset>(dataset_xptr);
  
  // unpack window
  int offx = INTEGER(window)[0]; 
  int offy = INTEGER(window)[1]; 
  int xn = INTEGER(window)[2];
  int yn = INTEGER(window)[3];
  int nx = INTEGER(window)[4];
  int ny = INTEGER(window)[5];
  
  int nbands = dataset->GetRasterCount(); 
  //double * data[nx * ny * nbands]; 
  
  
  double *padScanline;
  int n = nx * ny * nbands; 
  padScanline = (double *) CPLMalloc(sizeof(double) * static_cast<size_t>(n));
  
  CPLErr err; 
  GDALRasterIOExtraArg psExtraArg;

  psExtraArg = init_resample_alg(resample_algo);
  
  SEXP result; 
  if (isNull(data)) {
    Rprintf("data is NULL\n"); 
    err = dataset->RasterIO( GF_Read , offx, offy, xn, yn,
                             &padScanline[0], nx, ny, GDT_Float64,
                             0, 0, 
                             0, 0, 0, &psExtraArg);
    
    result = PROTECT(Rf_allocVector(REALSXP, n));
    for (int i = 0; i < n; i++) {
      
      REAL(result)[i] = padScanline[i];
      
    }
    
  } else {
    Rprintf("data is REAL\n"); 
    // user passed in a real vector
    for (int i = 0; i < n; i++) {
      padScanline[i] = REAL(data)[i];
    }
    err = dataset->RasterIO( GF_Write, offx, offy, xn, yn,
                             &padScanline[0], nx, ny, GDT_Float64,
                             0, 0, 
                             0, 0, 0, &psExtraArg);
    
    result = PROTECT(Rf_allocVector(REALSXP, 1)); 
  }
  
  
  UNPROTECT(1); 
  return result;
}
// standalone
extern "C" SEXP GdalPtrVSIReadDirRecursive(SEXP dsn_name_sexp) {
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




extern "C" SEXP GdalPtrGDALVersionInfo() {
  
  SEXP result = PROTECT(Rf_allocVector(STRSXP, 1));
  SET_STRING_ELT(result, 0, Rf_mkChar(GDALVersionInfo("--version")));
  UNPROTECT(1); 
  return result;
}


