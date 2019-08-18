//forward declarations of test routines
int test_convolve();
void test_resample();
int test_kernel();

int main() {
    test_kernel();
    test_convolve();
    test_resample();
    return 0;
}