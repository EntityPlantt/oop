// 1.5
struct date {
	// ISO8601
	short year, month, day;
};
struct footballer {
	// new tech: variable length, also assignment works! marvelous!
	const char *name, *surname, *clubname;
	date birthdate;
};
