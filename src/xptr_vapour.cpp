#include <Rcpp.h>
using namespace Rcpp;
#include "gdal_priv.h"
#include "ogrsf_frmts.h"

// LinkingTo:  vapour
#include "gdalraster/gdalraster.h"

//' Open a gdal dataset
//'
//' @param dsn data source name (file path, url,etc)
//' @param sds subdataset to choose (1-based, or defaults to 1 if not specified or if there are no sds)
//' @return external pointer
//' @export
// [[Rcpp::export]]
SEXP xptr_GDALOpen(CharacterVector dsn, IntegerVector sds = IntegerVector::create(0)) {
  GDALAllRegister();
  GDALDataset       *poDS;
  Rprintf("%i\n", sds[0]);
  poDS = (GDALDataset*)gdalraster::gdalH_open_dsn(dsn[0], sds); 
  // create pointer to an GDAL object and
  // wrap it as an external pointer
  
  if( poDS == nullptr)
  {
    Rprintf("Problem with 'dsn' input: %s\n", dsn[0]);
    Rcpp::stop("Open failed.\n");
  }
  Rcpp::XPtr<GDALDataset> ptr(poDS);
  
  return ptr;
}

//' Query the raster size a gdal dataset 
//'
//' @param xp external pointer returned by [xptr_GDALOpen()]
//' @return ncol, nrow of underlying data set
//' @export
// [[Rcpp::export]]
SEXP xptr_GDALGetRasterSize(SEXP xp) {
  
  Rcpp::XPtr<GDALDataset> ptr(xp);  // create pointer to an GDAL object and
  
  NumericVector res(2) ;
  
  res[0] = ptr->GetRasterXSize(); 
  res[1] = ptr->GetRasterYSize(); 
  
  return res; 
}
