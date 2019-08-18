/// \file
/// \brief Kernel test file
/// \author Hailin Jin
/// \date 2006 \n Last updated on October 12, 2006

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/gil/extension/numeric/kernel.hpp>

int test_kernel() {
    try {
    using namespace boost::gil;

    detail::kernel_1d_adaptor<std::vector<int> > k3_1(10,2);
    detail::kernel_1d_adaptor<std::vector<int> > k3_2(10,2);
    std::size_t k3_2_size=k3_2.size(); k3_2_size=1;
    k3_2.begin();
    k3_2.end();
    k3_2[0];
    k3_2.left_size();
    k3_2.right_size();
    k3_2.center();
    k3_2.center()=1;
    std::swap(k3_1,k3_2);

    kernel_1d<int> k4_1(10,2);
    kernel_1d<int> k4_2(10,3);
    std::swap(k4_1,k4_2);
    kernel_1d<int> k4_3(reverse_kernel(k4_2));

    kernel_1d_fixed<int,10> k5_1(2);
    std::fill_n(k5_1.begin(),10,1);
    kernel_1d_fixed<int,10> k5_2(2);
    std::fill_n(k5_2.begin(),10,2);
    std::swap(k5_1,k5_2);
    kernel_1d_fixed<int,10> k5_3(reverse_kernel(k5_1));

    return 1;

    } catch (...) {
    return 0;
    }
}
