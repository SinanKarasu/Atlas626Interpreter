#ifndef VertexDictionary_H
#define VertexDictionary_H


class VertexDictionary : public RWTValHashDictionary< RWCString,Vertex *> {
	public:
		VertexDictionary();

	private:
		enum { NbrBuckets = RWDEFAULT_CAPACITY };
};

class VertexDictionaryIterator : public RWTValHashDictionaryIterator< RWCString,Vertex *> {
        public: VertexDictionaryIterator(VertexDictionary &d);
};

#endif // VertexDictionary_H
