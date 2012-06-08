#ifndef _GB_CARTIDGE_
#define _GB_CARTIDGE_

struct gb_cartidge *load_cartidge(const char *filename);

int8_t cartidge_read(struct gb_cartidge* cartidge, int memAddr);

void cartidge_write(struct gb_cartidge *cartidge, int memAddr, int8_t write_data);

void cartidge_print(struct gb_cartidge *cartidge);

#endif
