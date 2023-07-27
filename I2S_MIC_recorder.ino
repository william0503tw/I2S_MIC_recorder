// ffmpeg -f s32le -ar 44100 -ac 1 -i track.i2s track.wav
#include <SD.h>
#include <driver/gpio.h>
#include <driver/i2s.h>

#define I2S_SAMPLE_RATE   (44100)
#define I2S_SAMPLE_BITS   (16)
//#define RECORD_TIME       (10) * 2 //Seconds
#define FLASH_RECORD_SIZE(rec_sec) (1 * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * rec_sec)

static const int SD_CS = 26;

File f;

String filename = "track";
String record_time = "0";
int totalBytesRead = 0;
int RECORD_TIME = 0 ;
int bytesToRead = 0 ;

void setup(){
  Serial.begin(115200);

  i2s_config_t i2s_config;
  i2s_config.mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_RX);
  i2s_config.sample_rate = I2S_SAMPLE_RATE;
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
  i2s_config.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT;
  i2s_config.communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
  i2s_config.dma_buf_count = 32;
  i2s_config.dma_buf_len = 32 * 2;
  i2s_config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;

  i2s_pin_config_t pin_config;
  pin_config.bck_io_num = 14;
  pin_config.ws_io_num = 15;
  pin_config.data_out_num = I2S_PIN_NO_CHANGE;
  pin_config.data_in_num = 32;

  i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &pin_config);
  i2s_stop(I2S_NUM_1);

  i2s_start(I2S_NUM_1);
}

void loop(){
  while(1){
    char c = Serial.read();
    if(c == 'a'){
      break ;
    }else{
      Serial.print('s');
      delay(1000);
    }
  }
  Serial.print('\n');

  // ---- File name ----
  Serial.print("Enter File Name: ");
  filename = Serial.readString();
  while(filename == ""){
    filename = Serial.readString();
  }
  Serial.println(filename);

  SD.begin(SD_CS);
  f = SD.open(String("/" + filename + ".i2s"), FILE_WRITE);

  if(!f){
    Serial.println("Failed : Remember to inset SD card");
    while(1);
  }

  // ---- Recoed second ----
  Serial.print("Enter Record time second: ");
  record_time = Serial.readString();
  while(record_time == ""){
    record_time = Serial.readString() ;
    RECORD_TIME = record_time.toInt();
    bytesToRead = FLASH_RECORD_SIZE(RECORD_TIME);
  }
  Serial.println(RECORD_TIME);

  Serial.println("**** Start Recording ****"); 
  Serial.println(String(" Writing file into /" + filename + ".i2s | sec: " + RECORD_TIME + 's'));
  totalBytesRead = 0 ;


  while(1){
    
    if (totalBytesRead >= bytesToRead) {
      break;
    }
    uint8_t buf[64];
    memset(buf, 0, 64);
    int bytes_read = 0;
    while(bytes_read == 0) {
      bytes_read = i2s_read_bytes(I2S_NUM_1, (char*) buf, 64, 0);
    }
    f.write(buf, 64);
    totalBytesRead += 64;
    if (totalBytesRead >= bytesToRead) {
      f.close();
      Serial.println("**** DONE WRITING ****");
      Serial.print('\n');
      listDir(SD, "/", 0);
      Serial.println("");
      break;
    }
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
