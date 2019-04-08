#ifndef PATTERN_H_
#define PATTERN_H_
#include <hdf5.h>

char *pattern1(hid_t *dset_arr, int dsize, int dim0, int dim1);
char *pattern2(hid_t *dset_arr, int dsize, int dim0, int dim1);
char *pattern3(hid_t *dset_arr, int dsize, int dim0, int dim1);

#endif /* ifndef PATTERN_H_ */
