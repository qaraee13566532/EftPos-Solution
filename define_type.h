
union IntChar
{
	uint16_t  intno;
	uint8_t byteno[2];
};

union CalibFact
{
	double   factor;
	uint8_t byteno[4];
};

union LongChar
{
	double   factor;
	int32_t    longno; 
	uint8_t byteno[4]; 
}; 

struct SettingParameters
{
    uint32_t  IsConfig;
    uint32_t  Out_PosBaudRate;
    uint32_t  In_PosBaudRate;
    uint8_t   Out_EftPo_Type;
    uint8_t   Toman_Mode;
    uint8_t   In_EftPo_Type;
    uint8_t   reserve_3;

};

            
union SetParameter
{
    struct SettingParameters Set_Parameter ;
    uint32_t Set_Parameter_Words[sizeof(struct SettingParameters)/4];
};

