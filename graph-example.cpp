#include <iostream>

#include <matplot/matplot.h>
#include <vector>   
#include <chrono>
#include <thread>

int main() {
    auto f = matplot::figure(false);
    f->backend()->run_command("unset warnings");
    f->ioff();
    f->size(1600, 900);
    // Use steady_clock for duration measurements as it is monotonic
    auto start = std::chrono::steady_clock::now(); // Record start time
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    auto end = std::chrono::steady_clock::now(); // Record end time

    // Calculate the duration and cast to desired units (e.g., milliseconds, microseconds, seconds)
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Output the result
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {1.5, 3.5, 2.5, 4.5, 3.0};

    matplot::plot(x, y, "-o"); // Plot with circles connected by a line
    matplot::title("Native Matplot++ Scatter Plot");
    matplot::xlabel("X Axis");
    matplot::ylabel("Y Axis");
    matplot::show();

    

    return 0;
}

