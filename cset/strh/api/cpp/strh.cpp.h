#ifndef STRH_CPP
#define STRH_CPP 1

class Strh {
private:
       void *data;
public:
	Strh(const void *str);
	int equ(const void *str);
	int chk(const void *str, size_t offset = 0);
	size_t *indexOf(const void *needle, size_t at);
	void put(const void *object, size_t at);
	struct utf8 {
		static size_t w(const void *ch);
		static size_t len(const void *str);
	};
	size_t bytelen();
	size_t utf8len();
	void *get();
	~Strh();
};

Strh::Strh(const void *str) { data = strh_create(str); }
Strh::~Strh() { free(data); }
size_t Strh::utf8::w(const void *ch) {return strh_utf8_width(ch); }
size_t Strh::utf8::len(const void *str) {return strh_utf8_len(str); }
size_t Strh::bytelen() { return strlen((char *)data); }
size_t Strh::utf8len() { return strh_utf8_len(data); }

int Strh::equ(const void *str) { return strh_equ(data, str); }
int Strh::chk(const void *str, size_t offset) {
	unsigned char *s = (unsigned char *)data;
	return strh_chk(s + offset, str);
}
size_t *Strh::indexOf(const void *needle, size_t at) {
	return strh_indexOf(data, needle, at);
}
void Strh::put(const void *object, size_t at) { strh_put(&data, object, at); }
void *Strh::get() { return data; }

#endif /* STRH_CPP */
