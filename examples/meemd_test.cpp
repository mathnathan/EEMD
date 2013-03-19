#include "meemd.h"

int main() {

    const std::string netcdf_file_cropped = "../data/crop.png";
    const std::string netcdf_file = "../data/sschlor.ncom.GOMh0.04.012.nest1.2009101000_00000000.nc";
    const std::string simple_netcdf_file = "../data/simple_xy.nc";
    const std::string lena_small = "../data/lena_small.bmp";
    const std::string lena_big = "../data/lena.bmp";

    MEEMD m;

    //m.load( netcdf_file, MEEMD_NETCDF );
    //m.load( simple_netcdf_file, MEEMD_NETCDF );
    m.load( lena_big, MEEMD_IMAGE );
    //m.load( lena_small, MEEMD_IMAGE );
    //m.load( netcdf_file_cropped, MEEMD_IMAGE );

    float noise_amplitude = 0.1;
    int nb_imfs = 6;
    int nb_noise_iters = 100;

    m.decompose( noise_amplitude, nb_imfs, nb_noise_iters );
    m.combine();
    m.show();

    const std::string save_dir = "../results/03-18_lena-big_noise=0.1/";
    m.save( save_dir );

    return 0;
}
