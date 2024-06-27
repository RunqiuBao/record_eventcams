#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <vector>
#include <metavision/sdk/driver/camera.h>
#include <metavision/hal/facilities/i_trigger_in.h>


using namespace std;
namespace fs = std::filesystem;

std::string GetCurrentTimeInString() {
    auto now = std::chrono::system_clock::now();
    auto now_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_lt = *std::localtime(&now_t);

    std::ostringstream oss;
    oss << std::put_time(&now_lt, "%Y-%m-%dT%H_%M_%S");
    return oss.str();
}

int main(int argc, char** argv){

    if (argc != 3){
        cout<< "need to set output_data_rootuni, timespan(ms)" << endl;
        return 0;
    }

    string stringPathToDataRoot = string(argv[1]);
    int timespan = std::stoi(std::string(argv[2]));
    
    cout << "Start capture for " << std::to_string(timespan) << " miliseconds (prophesee), \n" << endl;

    // prophesee camera
    Metavision::AvailableSourcesList availableList = Metavision::Camera::list_online_sources();
    std::vector<string> camIdList = availableList[Metavision::OnlineSourceType::USB];
    for (auto camId : camIdList){
        cout << "Camera ID: " << camId << endl;
    }

    Metavision::Camera leftCam = Metavision::Camera::from_serial(camIdList[0]);
    leftCam.get_device().get_facility<Metavision::I_TriggerIn>()->enable(Metavision::I_TriggerIn::Channel::Main);  // enable external trigger
    Metavision::Camera rightCam = Metavision::Camera::from_serial(camIdList[1]);
    rightCam.get_device().get_facility<Metavision::I_TriggerIn>()->enable(Metavision::I_TriggerIn::Channel::Main);  // enable external trigger

    // create output folder
    fs::path dataRoot = stringPathToDataRoot;
    fs::path seqPath = dataRoot / GetCurrentTimeInString();
    fs::create_directories(seqPath);

    // record event sequences here.
    fs::path pathToLeftEvent(seqPath / "left.raw");
    leftCam.start();
    leftCam.start_recording(pathToLeftEvent);
    fs::path pathToRightEvent(seqPath / "right.raw");
    rightCam.start();
    rightCam.start_recording(pathToRightEvent);

    std::this_thread::sleep_for(std::chrono::milliseconds(timespan));
    cout << "stopping recording..."  << endl;

    leftCam.stop_recording();
    rightCam.stop_recording();
    leftCam.stop();
    rightCam.stop();

    cout << "Recording finished normally" << endl;

}