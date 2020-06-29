#include "XMLParser.h"

#include "DataStructures/StringAlgorithm.h"
#include "DataStructures/ElementComparators.h"

namespace _GameEngine::_Utils
{
	short XMLNode_StartIndexOrdering(XMLNode* p_left, XMLNode* p_right, void* p_null)
	{
		return _Core::SizeTSortCompararator(p_left->XmlBeginIndex, p_right->XmlBeginIndex);
	}
}

namespace _GameEngine::_Utils
{
	bool XMLNode_comparator(XMLNode* p_left, XMLNode* p_right)
	{
		return p_left == p_right;
	}
}

namespace _GameEngine::_Utils
{
	XMLGraphIterator XMLGraphIterator_build(XMLGraph* p_graph)
	{
		return
		{
			&p_graph->RootNode,
			nullptr
		};
	};

	XMLGraphIterator XMLGraphIterator_build(XMLNode* p_startNode)
	{
		return
		{
			p_startNode,
			nullptr
		};
	};

	bool XMLGraphIterator_moveNext(XMLGraphIterator* p_graphIterator)
	{
		if (!p_graphIterator->CurrentNode) { p_graphIterator->CurrentNode = p_graphIterator->RootNode; }
		else
		{
			if (p_graphIterator->CurrentNode == p_graphIterator->RootNode)
			{
				p_graphIterator->CurrentNode = p_graphIterator->CurrentNode->Childs.at(0);
			}
			else
			{
				size_t l_index = p_graphIterator->CurrentNode->Parent->Childs.getIndex(XMLNode_comparator, p_graphIterator->CurrentNode);
				if (l_index < p_graphIterator->CurrentNode->Parent->Childs.size() - 1)
				{
					p_graphIterator->CurrentNode = p_graphIterator->CurrentNode->Parent->Childs.at(l_index + 1);
				}
				else
				{
					p_graphIterator->CurrentNode = p_graphIterator->CurrentNode->Parent;
				}
			}



			if (p_graphIterator->CurrentNode == p_graphIterator->RootNode)
			{
				p_graphIterator->CurrentNode = nullptr;
			}
		}

		return p_graphIterator->CurrentNode;
	};

	bool XMLGraphIterator_skipCurrentChildrens(XMLGraphIterator* p_graphIterator)
	{
		if (p_graphIterator->CurrentNode)
		{
			p_graphIterator->CurrentNode = p_graphIterator->CurrentNode->Parent;
		}
		return p_graphIterator->CurrentNode;
	};

	_Core::FixedString CHAR_INFERIOR = _Core::FixedString_interpret("<");
	_Core::FixedString CHAR_SUPERIOR = _Core::FixedString_interpret(">");

	struct XMLBalise
	{
		bool IsStart;
		size_t BeginIndex;
		size_t EndIndex;
		_Core::String BaliseContent;
	};

	void XMLBalise_allocFromString(XMLBalise* p_xmlBalise, _Core::String* p_baliseFullString, size_t p_beginIndex, size_t p_endIndex)
	{
		p_xmlBalise->IsStart = !(*p_baliseFullString->Vector.at(1) == '/');
		p_xmlBalise->BaliseContent.alloc((p_endIndex - p_beginIndex));
		if (p_xmlBalise->IsStart)
		{
			_Core::String_substring(p_baliseFullString, 1, (p_endIndex - p_beginIndex) - 1, &p_xmlBalise->BaliseContent);
		}
		else
		{
			_Core::String_substring(p_baliseFullString, 2, (p_endIndex - p_beginIndex) - 1, &p_xmlBalise->BaliseContent);
		}

		p_xmlBalise->BeginIndex = p_beginIndex;
		p_xmlBalise->EndIndex = p_endIndex;
	};

	void XMLBalise_free(XMLBalise* p_xmlBalise, void* p_null)
	{
		p_xmlBalise->BaliseContent.free();
	};

	void XMLAttribute_allocFromString(XMLAttribute* p_xmlAttribute, _Core::String* p_attributeFullString)
	{

		size_t l_equalIndex;
		if (_Core::String_find(p_attributeFullString, "=", 1, 0, &l_equalIndex))
		{
			p_xmlAttribute->Name.alloc(l_equalIndex);
			_Core::String_substring(p_attributeFullString, 0, l_equalIndex - 1, &p_xmlAttribute->Name);
			size_t l_attributeValueBeginIndex, l_attributeValueEndIndex;
			if (_Core::String_find(p_attributeFullString, "\"", 1, 0, &l_attributeValueBeginIndex) && _Core::String_find(p_attributeFullString, "\"", 1, l_attributeValueBeginIndex + 1, &l_attributeValueEndIndex))
			{
				p_xmlAttribute->Value.alloc(l_attributeValueEndIndex - l_attributeValueBeginIndex);
				_Core::String_substring(p_attributeFullString, l_attributeValueBeginIndex + 1, l_attributeValueEndIndex - 1, &p_xmlAttribute->Value);
			}
		}
	};

	void XMLAttribute_free(XMLAttribute* p_attr, void* p_null)
	{
		p_attr->Name.free();
		p_attr->Value.free();
	};

	bool XMLGraph_findNextBalise(_Core::String* p_input, size_t p_startIndex, XMLBalise* out_xmlBalise)
	{
		size_t l_nodeBeginXMLBaliseStart;
		if (STRING_FIND(p_input, &CHAR_INFERIOR, p_startIndex, &l_nodeBeginXMLBaliseStart))
		{
			size_t l_nodeBeginXMLBaliseEnd;
			if (STRING_FIND(p_input, &CHAR_SUPERIOR, l_nodeBeginXMLBaliseStart + 1, &l_nodeBeginXMLBaliseEnd))
			{
				_Core::String l_balise;
				l_balise.alloc(0);
				_Core::String_substring(p_input, l_nodeBeginXMLBaliseStart, l_nodeBeginXMLBaliseEnd, &l_balise);
				XMLBalise_allocFromString(out_xmlBalise, &l_balise, l_nodeBeginXMLBaliseStart, l_nodeBeginXMLBaliseEnd);
				return true;
			}
		}

		return false;
	}

	void XMLNode_alloc(XMLNode* p_node, XMLBalise* p_begin, XMLBalise* p_end, _Core::String* p_xml)
	{
		p_node->Content.alloc(0);
		_Core::String_substring(p_xml, p_begin->EndIndex + 1, p_end->BeginIndex - 1, &p_node->Content);

		p_node->XmlBeginIndex = p_begin->BeginIndex;
		p_node->XMlEndIndex = p_end->EndIndex;

		p_node->Attributes.alloc();
		//Extracting name and attributes
		{
			bool l_nameFound = false;
			bool l_iterating = true;
			size_t l_beginBaliseContentCursor = 0;
			size_t l_spaceFoundIndex;
			while (l_iterating)
			{
				bool l_spaceFound = _Core::String_find(&p_begin->BaliseContent, " ", 1, l_beginBaliseContentCursor, &l_spaceFoundIndex);
				if (!l_spaceFound)
				{
					if (l_beginBaliseContentCursor <= (_Core::String_charNb(&p_begin->BaliseContent) - 1))
					{
						l_spaceFoundIndex = _Core::String_charNb(&p_begin->BaliseContent);
					}
					else
					{
						l_iterating = false;
					}
				}

				if (l_iterating)
				{
					if (!l_nameFound)
					{
						p_node->Name.alloc((l_spaceFoundIndex - l_beginBaliseContentCursor));
						_Core::String_substring(&p_begin->BaliseContent, l_beginBaliseContentCursor, l_spaceFoundIndex - 1, &p_node->Name);

						l_nameFound = true;
					}
					else
					{
						char* l_attributeStringBegin = p_begin->BaliseContent.Vector.at(l_beginBaliseContentCursor);
						_Core::String singleAttributeSlice = _Core::String_interpret(l_attributeStringBegin, (l_spaceFoundIndex - l_beginBaliseContentCursor));
						{
							XMLAttribute l_attribute;
							XMLAttribute_allocFromString(&l_attribute, &singleAttributeSlice);
							p_node->Attributes.push_back(&l_attribute);
						}
						singleAttributeSlice.free();
					}

					l_beginBaliseContentCursor = l_spaceFoundIndex + 1;
				}
			}

		}

		p_node->Childs.alloc();
	};

	void XMLNode_free(XMLNode* p_node)
	{
		p_node->Name.free();
		p_node->Content.free();
		p_node->Attributes.forEach(XMLAttribute_free, (void*)nullptr);
		p_node->Attributes.free();
		p_node->Childs.free();
	};

	void XMLNode_freeRecursive(XMLNode* p_node, void*)
	{
		p_node->Childs.forEach(XMLNode_freeRecursive, (void*)nullptr);
		XMLNode_free(p_node);
	};

	void XMLGraph_parse(_Core::String* p_input, XMLGraph* p_outGraph)
	{
		size_t l_nodeStartIndex = 0;
		size_t l_nodeEndIndex = 0;

		_Core::VectorT<XMLBalise> XmlBalises;
		XmlBalises.alloc();
		{
			size_t l_xmlCursor = 0;
			XMLBalise l_balise{};
			while (XMLGraph_findNextBalise(p_input, l_xmlCursor, &l_balise))
			{
				XmlBalises.push_back(&l_balise);
				l_xmlCursor = l_balise.EndIndex;
			}



			_Core::VectorT<XMLNode> InstanciatedXMLNodeStack;
			InstanciatedXMLNodeStack.alloc();

			// XML Node instanciation
			{
				_Core::VectorT<size_t> XmlOpenBaliseIndicesStack;
				XmlOpenBaliseIndicesStack.alloc(XmlBalises.size());
				{
					for (size_t i = 0; i < XmlBalises.size(); i++)
					{
						XMLBalise* p_xmlBalise = XmlBalises.at(i);
						if (p_xmlBalise->IsStart)
						{
							XmlOpenBaliseIndicesStack.push_back(&i);
						}
						else
						{
							size_t l_beginBaliseArrayIndex = *XmlOpenBaliseIndicesStack.at(XmlOpenBaliseIndicesStack.size() - 1);

							XMLBalise* p_beginXMLBalise = XmlBalises.at(l_beginBaliseArrayIndex);
							XMLNode l_newNode{};
							XMLNode_alloc(&l_newNode, p_beginXMLBalise, p_xmlBalise, p_input);
							InstanciatedXMLNodeStack.push_back(&l_newNode);
							XmlOpenBaliseIndicesStack.erase(XmlOpenBaliseIndicesStack.size() - 1);
						}
					}
				}
				XmlOpenBaliseIndicesStack.free();
			}

			// XML Node hierarchy
			{
				InstanciatedXMLNodeStack.selectionSort(XMLNode_StartIndexOrdering, (void*)nullptr);

				// Setting childs
				{
					for (size_t l_childIndex = InstanciatedXMLNodeStack.size() - 1; l_childIndex < InstanciatedXMLNodeStack.size(); l_childIndex--)
					{
						XMLNode* l_childNode = InstanciatedXMLNodeStack.at(l_childIndex);
						if (l_childIndex == 0)
						{
							p_outGraph->RootNode = *l_childNode;
						}
						else
						{
							for (size_t l_parentIndex = l_childIndex - 1; l_parentIndex < l_childIndex; l_parentIndex--)
							{
								XMLNode* l_potentialParentNode = InstanciatedXMLNodeStack.at(l_parentIndex);
								if (l_potentialParentNode->XMlEndIndex > l_childNode->XMlEndIndex)
								{
									l_potentialParentNode->Childs.push_back(l_childNode);
								}
							}
						}
					}
				}

				// Setting parents. We iterate on the final XMLGraph because pointer to pointer nodes must point to memory of the actual graph and not the temprary stack created from raw string.
				{
					struct IteratingNodeStack
					{
						XMLNode* Node;
						size_t ChildNodeIndex;
					};

					_Core::VectorT<IteratingNodeStack> l_interatingNodeStack;
					l_interatingNodeStack.alloc();
					{
						IteratingNodeStack l_root = { &p_outGraph->RootNode, 0 };
						l_interatingNodeStack.push_back(&l_root);
					}

					{
						while (l_interatingNodeStack.size() > 0)
						{
							IteratingNodeStack* l_currentIteratingNode = l_interatingNodeStack.at(l_interatingNodeStack.size() - 1);

							if (l_currentIteratingNode->ChildNodeIndex < l_currentIteratingNode->Node->Childs.size())
							{
								XMLNode* l_childNode = l_currentIteratingNode->Node->Childs.at(l_currentIteratingNode->ChildNodeIndex);
								l_childNode->Parent = l_currentIteratingNode->Node;
								l_currentIteratingNode->ChildNodeIndex++;
								IteratingNodeStack l_oneLevelDeepStack = { l_childNode, 0 };
								l_interatingNodeStack.push_back(&l_oneLevelDeepStack);
							}
							else
							{
								l_interatingNodeStack.erase(l_interatingNodeStack.size() - 1);
							}
						}
					}
					l_interatingNodeStack.free();
				}
			}

			// Individual nodes are not disposed because they are copied recursively to p_outGraph
			InstanciatedXMLNodeStack.free();

			InstanciatedXMLNodeStack.free();
		}
		XmlBalises.forEach(XMLBalise_free, (void*)nullptr);
		XmlBalises.free();


	};

	void XMLGraph_free(XMLGraph* p_xmlGraph)
	{
		XMLNode_freeRecursive(&p_xmlGraph->RootNode, (void*)nullptr);
	};
}