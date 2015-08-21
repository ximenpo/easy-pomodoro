
#include  <EEPROM.h>
#include  <checksum.h>

static  const size_t  STORAGE_SIZE    = 512;
static  const char    STORAGE_TAG[4]  = {'E', 'P', 'G', 'D',};
struct  StorageHead {
  char      tag[4];               //  EPGD ->  easy-pomodoro GTD
  uint16_t  data_size;
  uint16_t  check_sum;
} __attribute__((__packed__));


bool  storage_load(uint8_t* begin, uint8_t* end) {
  //assert((begin < end) && (begin + STORAGE_SIZE - sizeof(StorageHead) < end));

  EEPROM.begin(STORAGE_SIZE);

  StorageHead  head;

  //  read head
  for (size_t i = 0; i < sizeof(head); ++i) {
    *((uint8_t*)&head)  = EEPROM.read(i);
  }

  if ( (0 != memcmp(head.tag, STORAGE_TAG, sizeof(STORAGE_TAG)))
       || (head.data_size > STORAGE_SIZE - sizeof(StorageHead))
     ) {
    return  false;
  }

  size_t  data_size  = end - begin;
  if (data_size > head.data_size) {
    data_size  = head.data_size;
  }

  for (size_t i = 0; i < data_size; ++i) {
    *(begin + i)  = EEPROM.read(sizeof(head) + i);
  }
  EEPROM.end();

  return  (head.check_sum == checksum(begin, head.data_size));
}

void  storage_save(uint8_t* begin, uint8_t* end) {
  //assert((begin < end) && (begin + STORAGE_SIZE - sizeof(StorageHead) < end));

  EEPROM.begin(STORAGE_SIZE);
  StorageHead   head;
  memcpy(head.tag, STORAGE_TAG, sizeof(STORAGE_TAG));
  head.data_size  = end - begin;
  head.check_sum  = checksum(begin, head.data_size);

  uint8_t*  pHead = (uint8_t*)&head;
  for (size_t i = 0; i < sizeof(head); ++i) {
    EEPROM.write(i, *(pHead + i));
  }

  uint8_t*  pData = (uint8_t*)&head;
  for (size_t i = 0; i < head.data_size; ++i) {
    EEPROM.write(i + sizeof(head), *(begin + i));
  }
  EEPROM.end();
}

