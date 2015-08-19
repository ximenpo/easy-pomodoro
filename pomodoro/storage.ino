
#include  <EEPROM.h>
#include  <checksum.h>

static  const size_t  STORAGE_SIZE    = 512;
static  const char    STORAGE_TAG[4]  = {'E', 'P', 'G', 'D',};
struct  StorageHead {
  char      tag[4];               //  EPGD ->  easy-pomodoro GTD
  uint16_t  data_size;
  uint16_t  check_sum;
} __attribute__((__packed__));


size_t  storage_load(uint8_t* begin, uint8_t* end) {
  //assert((begin < end) && (begin + STORAGE_SIZE - sizeof(StorageHead) < end));

  EEPROM.begin(STORAGE_SIZE);
  uint8_t*      pStorage  = EEPROM.getDataPtr();

  StorageHead*  pHead   = (StorageHead*)&pStorage;
  if ( (0 != memcmp(pHead->tag, STORAGE_TAG, sizeof(STORAGE_TAG)))
       || (pHead->data_size > STORAGE_SIZE - sizeof(StorageHead))
       || (pHead->check_sum != checksum(pStorage + sizeof(StorageHead), pHead->data_size))
     ) {
    return  0;
  }

  size_t  data_size  = end - begin;
  if (data_size > pHead->data_size) {
    data_size  = pHead->data_size;
  }

  memcpy(begin, pStorage + sizeof(StorageHead), data_size);
  EEPROM.end();

  return  data_size;
}

void  storage_save(uint8_t* begin, uint8_t* end) {
  //assert((begin < end) && (begin + STORAGE_SIZE - sizeof(StorageHead) < end));

  EEPROM.begin(STORAGE_SIZE);
  uint8_t*      pStorage  = EEPROM.getDataPtr();

  StorageHead*  pHead   = (StorageHead*)&pStorage;
  memcpy(pHead->tag, STORAGE_TAG, sizeof(STORAGE_TAG));
  pHead->data_size  = end - begin;
  pHead->check_sum  = checksum(begin, pHead->data_size);
  memcpy(pStorage + sizeof(StorageHead), begin, pHead->data_size);
  EEPROM.end();
}

