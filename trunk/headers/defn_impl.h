#ifndef DEFN_IMPL_H
#define	DEFN_IMPL_H

#ifdef	__cplusplus
extern "C" {
#endif

struct Word {
	char *str;
	int size;
};

struct Query {
    int id;
    char *str;
    enum MatchType match_type;
    unsigned int match_dist;
};

struct Document {
    int id;
    char *str;
};

#ifdef	__cplusplus
}
#endif

#endif	/* DEFN_IMPL_H */
