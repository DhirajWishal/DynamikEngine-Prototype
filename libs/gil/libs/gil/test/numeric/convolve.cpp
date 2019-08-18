/// \file
/// \brief Convolution test file
/// \author Hailin Jin
/// \date 2006-2007 \n Last updated on February 16, 2007

#include <cstddef>
#include <iostream>
#include <algorithm>
#include <boost/gil/gray.hpp>
#include <boost/gil/pixel.hpp>
#include <boost/gil/locator.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/pixel_iterator.hpp>
#include <boost/gil/step_iterator.hpp>
#include <boost/gil/iterator_from_2d.hpp>
#include <boost/gil/extension/numeric/kernel.hpp>
#include <boost/gil/extension/numeric/convolve.hpp>

#include "convolve-data.h"

int test_convolve() {
    using namespace boost::gil;

    kernel_1d<bits16> k(left+right+1,left);
    kernel_1d_fixed<bits16,left+right+1> k_fixed(left);
    std::copy(k_in0,k_in0+k.size(),k.begin());
    typedef pixel<bits16,gray_layout_t> Pixel;
    typedef memory_based_2d_locator<memory_based_step_iterator<Pixel*> > Loc;
    //define the input and copy from the .h file
    image<Pixel,false> im_in(width,height);
    std::copy((Pixel*)im_in0[0],(Pixel*)im_in0[0]+width*height,view(im_in).begin());
    image<Pixel,false> im_out(width,height);
    std::copy((Pixel*)im_out0[0],(Pixel*)im_out0[0]+width*height,view(im_out).begin());
    image<Pixel,false> im_padded_in(width+left+right,height);
    std::copy((Pixel*)im_in_padded[0],(Pixel*)im_in_padded[0]+(width+left+right)*height,view(im_padded_in).begin());
    //define outputs from the C++ code
    image<Pixel,false> im1(width,height),im2(width,height),im3(width,height),im4(width,height),im5(width,height);

    //run variable-size convolution routines
    convolve_rows<Pixel>(const_view(im_in),k,view(im1),convolve_option_output_ignore);
    convolve_rows<Pixel>(const_view(im_in),k,view(im2),convolve_option_output_zero);
    convolve_rows<Pixel>(subimage_view(const_view(im_padded_in),right,0,width,height),k,view(im3),convolve_option_extend_padded);
    convolve_rows<Pixel>(const_view(im_in),k,view(im4),convolve_option_extend_zero);
    convolve_rows<Pixel>(const_view(im_in),k,view(im5),convolve_option_extend_constant);

    bool success1=false,success2=false,success3=false,success4=false,success5=false;
    //check convolve_option_output_ignore
    image_view<Loc> im_out_sub, im1_sub;
    im_out_sub=subimage_view(view(im_out),right,0,width-left-right,height);
    im1_sub=subimage_view(view(im1),right,0,width-left-right,height);
    success1=std::equal(im_out_sub.begin(),im_out_sub.end(),im1_sub.begin());
    std::cout<<(success1?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_output_ignore"<<std::endl;

    //check convolve_option_output_zero
    image_view<Loc> im2_sub=subimage_view(view(im2),right,0,width-left-right,height);
    image_view<Loc> im2_view=view(im2);
    success2=true;
    for(std::size_t ii=0;ii<right;++ii) {
        success2&=std::find_if(im2_view.col_begin(ii),im2_view.col_end(ii),
                               std::bind2nd(std::not_equal_to<Pixel>(),bits16(0)))
            ==im2_view.col_end(ii);
    }
    for(std::size_t ii=0;ii<left;++ii) {
        success2&=std::find_if(im2_view.col_begin(width-1-ii),im2_view.col_end(width-1-ii),
                               std::bind2nd(std::not_equal_to<Pixel>(),bits16(0)))
            ==im2_view.col_end(width-1-ii);
    }
    success2&=std::equal(im_out_sub.begin(),im_out_sub.end(),im2_sub.begin());
    std::cout<<(success2?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_output_zero"<<std::endl;

    //check convolve_option_extend_padded
    success3=std::equal(view(im3).begin(),view(im3).end(),(Pixel*)im_out_padded[0]);
    std::cout<<(success3?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_extend_padded"<<std::endl;

    //check convolve_option_extend_zero
    success4=std::equal(view(im_out).begin(),view(im_out).end(),view(im4).begin());
    std::cout<<(success4?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_extend_zero"<<std::endl;

    //check convolve_option_extend_constant
    success5=std::equal(view(im5).begin(),view(im5).end(),(Pixel*)im_out_constant[0]);
    std::cout<<(success5?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_extend_constant"<<std::endl;

    //run variable-size convolution routines
    std::copy(k.begin(),k.end(), k_fixed.begin());
    convolve_rows_fixed<Pixel>(const_view(im_in),k_fixed,view(im1),convolve_option_output_ignore);
    convolve_rows_fixed<Pixel>(const_view(im_in),k_fixed,view(im2),convolve_option_output_zero);
    convolve_rows_fixed<Pixel>(subimage_view(const_view(im_padded_in),right,0,width,height),k_fixed,view(im3),convolve_option_extend_padded);
    convolve_rows_fixed<Pixel>(const_view(im_in),k_fixed,view(im4),convolve_option_extend_zero);
    convolve_rows_fixed<Pixel>(const_view(im_in),k_fixed,view(im5),convolve_option_extend_constant);

    success1=false; success2=false; success3=false; success4=false; success5=false;
    //check convolve_option_output_ignore
    im_out_sub=subimage_view(view(im_out),right,0,width-left-right,height);
    im1_sub=subimage_view(view(im1),right,0,width-left-right,height);
    success1=std::equal(im_out_sub.begin(),im_out_sub.end(),im1_sub.begin());
    std::cout<<(success1?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_output_ignore"<<std::endl;

    //check convolve_option_output_zero
    im2_sub=subimage_view(view(im2),right,0,width-left-right,height);
    im2_view=view(im2);
    success2=true;
    for(std::size_t ii=0;ii<right;++ii) {
        success2&=std::find_if(im2_view.col_begin(ii),im2_view.col_end(ii),
                              std::bind2nd(std::not_equal_to<Pixel>(),bits16(0)))
            ==im2_view.col_end(ii);
    }
    for(std::size_t ii=0;ii<left;++ii) {
        success2&=std::find_if(im2_view.col_begin(width-1-ii),im2_view.col_end(width-1-ii),
                              std::bind2nd(std::not_equal_to<Pixel>(),bits16(0)))
            ==im2_view.col_end(width-1-ii);
    }
    success2&=std::equal(im_out_sub.begin(),im_out_sub.end(),im2_sub.begin());
    std::cout<<(success2?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_output_zero"<<std::endl;

    //check convolve_option_extend_padded
    success3=std::equal(view(im3).begin(),view(im3).end(),(Pixel*)im_out_padded[0]);
    std::cout<<(success3?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_extend_padded"<<std::endl;

    //check convolve_option_extend_zero
    success4=std::equal(view(im_out).begin(),view(im_out).end(),view(im4).begin());
    std::cout<<(success4?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_extend_zero"<<std::endl;

    //check convolve_option_extend_constant
    success5=std::equal(view(im5).begin(),view(im5).end(),(Pixel*)im_out_constant[0]);
    std::cout<<(success5?"Passed":"Failed");
    std::cout<<": test for convolution option convolve_option_extend_constant"<<std::endl;

    return 0;
}
