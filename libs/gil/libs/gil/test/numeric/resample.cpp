#include <string>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/numeric/resample.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>

const std::string in_dir="../";
const std::string out_dir="../image-out/";
const std::string ref_dir="../image-ref/";

void test_resample() {
    using namespace boost::gil;
    
    rgb8_image_t img;
    jpeg_read_image(in_dir+"monkey.jpg",img);
    // test resize_view
    rgb8_image_t small_monkey(100,100);
    resize_view<bilinear_sampler>(const_view(img), view(small_monkey));
    jpeg_write_view(out_dir+"small_monkeyBL.jpg", view(small_monkey));

   // test resample_pixels
    rgb8_image_t transf(rgb8_image_t::point_t(iround(view(img).dimensions()/2)));
    matrix3x2<double> mat = matrix3x2<double>::get_translate(-point2<double>(100,50)) *
                            matrix3x2<double>::get_rotate(-15*3.14/180.0f);
    resample_pixels<nearest_neighbor_sampler>(const_view(img), view(transf), mat);
    jpeg_write_view(out_dir+"transformedNN.jpg", view(transf));
 
    // the sampler may be specified like this too
    resample_pixels(const_view(img), view(transf), mat, bilinear_sampler());
    jpeg_write_view(out_dir+"transformedBL.jpg", view(transf));
}
