#include "pattern.h"
#include <hdf5.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int fibonacci(int n) {
  if (n == 0 || n == 1)
    return n;
  else
    return (fibonacci(n - 1) + fibonacci(n - 2));
}

// slope
char *pattern1(hid_t *dset_arr, int dsize, int dim0, int dim1) {

  int wdata[dim0][dim1], i, j;
  herr_t status;
  time_t t;
  srand((unsigned)time(&t));

  for (i = 0; i < dim0 / 2; i++) {
    for (j = 0; j < dim1 / 2; j++) {
      int r = rand() % 20;
      wdata[i][j] = fibonacci(r);
    }
  }

  for (i = 0; i < dsize; i++) {
    for (j = 0; j < dsize; j++) {
      status = H5Dwrite(dset_arr[i], H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                        H5P_DEFAULT, wdata[0]);
    }
  }

  return "pattern1";
}

// could cache
char *pattern2(hid_t *dset_arr, int dsize, int dim0, int dim1) {

  int rdata[dim0][dim1], i, k;
  herr_t status;
  time_t t;
  srand((unsigned)time(&t));
  int repetitions = rand() % 4 + 2;
  for (k = 0; k < repetitions; k++) {
    for (i = 0; i < dsize; i++) {
      int rnd = rand() % 100;
      if (rnd % 3 == 0) {
        status = H5Dread(dset_arr[rand() % dsize], H5T_NATIVE_INT, H5S_ALL,
                         H5S_ALL, H5P_DEFAULT, rdata);
      } else {
        status = H5Dread(dset_arr[i], H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                         H5P_DEFAULT, rdata);
      }
    }
  }

  return "pattern2";
}

// WIP: read and write
char *pattern3(hid_t *dset_arr, int dsize, int dim0, int dim1) {

  int wdata[dim0][dim1], i, j;
  herr_t status;
  time_t t;
  srand((unsigned)time(&t));

  for (i = 0; i < dim0; i++)
    for (j = 0; j < dim1; j++)
      wdata[i][j] = i * j * pow(j, j);

  for (i = 0; i < dsize; i++) {
    status = H5Dread(dset_arr[i], H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                     wdata);

    if (i < dsize) {
      status = H5Dwrite(dset_arr[i + 1], H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                        H5P_DEFAULT, wdata[0]);
    }
  }

  return "pattern3";
}
