void aes_dec_128(uint8_t * cipherText, uint8_t* state , uint8_t* roundkeyarr );
void add_round_key(uint8_t* stateText, uint8_t* roundKey, uint8_t roundnum);
void inverse_subbytes(uint8_t* stateTextText, const uint8_t* aes_invsbox);
void inverse_shift_rows(uint8_t* stateTextText);
uint8_t gf256mul(uint8_t in1, uint8_t in2);
void inverse_mix_coloumns(uint8_t* s);
void gen_roundkey(uint8_t* key , uint8_t* k, const uint8_t* sboxarr, const uint8_t* rcon);

extern const uint8_t aes_sbox[];
extern const uint8_t aes_invsbox[];
extern const uint8_t rcon []; 
extern uint8_t cipherText [] ;
extern uint8_t	stateText [];
extern uint8_t roundkeyarr[];
extern uint8_t key[];

