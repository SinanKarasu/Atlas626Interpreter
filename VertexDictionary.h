#ifndef VertexDictionary_H
#define VertexDictionary_H


using VertexDictionary = AppendableMap<std::string, Vertex *>;

// class VertexDictionary : public RWTValHashDictionary< RWCString,Vertex *> {
// 	public:
// 		VertexDictionary();
// 
// 	private:
// 		enum { NbrBuckets = RWDEFAULT_CAPACITY };
// };

using VertexDictionaryIterator = VertexDictionary::iterator;
// class VertexDictionaryIterator : public RWTValHashDictionaryIterator< RWCString,Vertex *> {
//         public: VertexDictionaryIterator(VertexDictionary &d);
// };

#endif // VertexDictionary_H
