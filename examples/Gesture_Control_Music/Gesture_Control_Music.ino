#include "WT2605C_Player.h"
#include "Gesture.h"
#include <SoftwareSerial.h>

SoftwareSerial SSerial(2, 3); //use D2,D3 to simulate RX,TX
WT2605C<SoftwareSerial> Mp3Player;
pag7660 Gesture; // Combined mode is used by default

#define COMSerial SSerial
#define ShowSerial Serial


void setup() {
  ShowSerial.print(1);
  ShowSerial.begin(9600);
  COMSerial.begin(115200);
  Mp3Player.init(COMSerial);
  ShowSerial.println("initial finished");

  if(Gesture.init()) {
        ShowSerial.println("PAG7660 initialization success");
        ShowSerial.println("Please input your gestures:\n");
    } else {
        ShowSerial.println("PAG7660 initialization failed");
    }
}

void loop() {
    int ans;
    pag7660_gesture_t result;
    if (Gesture.getResult(result)) {
        ans = printResultCombinedMode(result);
        // ShowSerial.println("ans=");
        // ShowSerial.print(ans);
        playSong(ans);
    }
}

int playSong(int index){
    //if index equate to 0, then don't play song
    if(index==0){
      return 0;
    }
    Mp3Player.playSDRootSong(index);
    ShowSerial.println("Play music: " + String(index));
    return 1;
}

int printResultCombinedMode(const pag7660_gesture_t& result) {

    int ans = 0;
    const char *cursor_str[] = {
        NULL,
        "Tap",
        "Grab",
        "Pinch",
    };

    switch (result.type) {
    case 0:
        switch (result.cursor.type) {
        case 1:
        case 2:
        case 3:
            if (result.cursor.select)
                ShowSerial.println(cursor_str[result.cursor.type]);
            break;
        default:
            break;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        ans = result.type;
        ShowSerial.println("finger number");
        break;
    case 6:
        ans = result.type;
        ShowSerial.print("Rotate Right ");
        ShowSerial.println(result.rotate);
        break;
    case 7:
        ans = result.type;
        ShowSerial.print("Rotate Left ");
        ShowSerial.println(result.rotate);
        break;
    case 8:
        ans = result.type;
        ShowSerial.println("Swipe Left");
        break;
    case 9:
        ans = result.type;
        ShowSerial.println("Swipe Right");
        break;
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
        ans = result.type;
        ShowSerial.print(result.type - 19 + 1);
        ShowSerial.println("-finger push");
        break;
    default:
        break;
    }
    return ans;
}

