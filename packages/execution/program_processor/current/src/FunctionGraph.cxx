#include "execution/FunctionGraph.hpp"
#include "GraphCopier.hpp"
#include "execution/TaskAttribute.hpp"

#include "corelib/CommandNOP.hpp"
#include "corelib/ConditionFalse.hpp"
#include "corelib/ConditionTrue.hpp"
#include <boost/graph/copy.hpp>
#include <utility>

#include <iostream>

namespace ORO_Execution
{
    using namespace boost;
    using namespace std;
    using ORO_CoreLib::CommandNOP;
    using ORO_CoreLib::ConditionTrue;

    FunctionGraph::FunctionGraph(const std::string& _name)
        : myName(_name), error(false)
    {
        // the start vertex of our function graph
        start = add_vertex( program );
        put(vertex_exec, program, start, VertexNode::normal_node );
        exit = add_vertex( program );
        put(vertex_exec, program, exit, VertexNode::normal_node);
    }

    FunctionGraph::FunctionGraph( const FunctionGraph& orig )
        :  program( orig.getGraph() ), myName( orig.getName() ), error(false)
    {
        // The nodes are copied, which causes a clone of their contents.
        graph_traits<Graph>::vertex_iterator v1,v2, it;
        tie(v1,v2) = vertices(program);
        for ( it=v1; it != v2; ++it)
            if ( get( vertex_exec, program, *it) == VertexNode::func_start_node )
                break;
        start = *v1;
        for ( it=v1; it != v2; ++it)
            if ( get( vertex_exec, program, *it) == VertexNode::func_exit_node )
                break;
        exit = *v1;

        // Copy over the TAB pointers.
        args = orig.getArguments();
    }

    void FunctionGraph::finish()
    {
        put(vertex_exec, program, start, VertexNode::func_start_node );
        put(vertex_exec, program, exit, VertexNode::func_exit_node);

        // Because we use listS, we need to re-index the map :-(
        // If we do not do this, it can not be copied by the copy_graph
        // function.
        property_map<Graph, vertex_index_t>::type
            index = get(vertex_index, program);

        // initialize the vertex_index property values
        // so that it can be copied into other graphs.
        graph_traits<Graph>::vertex_iterator vi, vend;
        graph_traits<Graph>::vertices_size_type cnt = 0;
        for(tie(vi,vend) = vertices(program); vi != vend; ++vi)
            put(index, *vi, cnt++);
        this->reset();
    }

    FunctionGraph::~FunctionGraph()
    {
    }

    bool FunctionGraph::executeAll()
    {
        static const int maxsteps = 5000;
        int count = 0;
        bool result = true;

        while ( current != exit && count++ <= maxsteps && result )
            result = execute();
        return result;
    }

    bool FunctionGraph::execute()
    {
        graph_traits<Graph>::out_edge_iterator ei, ei_end;
        // the map contains _references_ to all vertex_command properties
        boost::property_map<Graph, vertex_command_t>::type
            cmap = get(vertex_command, program);
        boost::property_map<Graph, edge_condition_t>::type
            emap = get(edge_condition, program);

        // initialise current node if needed and reset all its out_edges
        if ( previous != current )
        {
            for ( tie(ei, ei_end) = boost::out_edges( current, program ); ei != ei_end; ++ei)
                emap[*ei].reset();
            cmap[current].startExecution();
            previous = current;
        }

        // execute the current command.
        if ( !cmap[current].execute() ) {
            error = true;
            return false;
        } else
            error = false; // it is possible to get out of error without resetting.

        // Branch selecting Logic :
        for ( tie(ei, ei_end) = boost::out_edges( current, program ); ei != ei_end; ++ei)
        {
            if ( emap[*ei].evaluate() )
            {
                current = boost::target(*ei, program);
                // a new node has been found ...
                // it will be executed in the next step.
                return true;
            }
        }
        return true; // no new branch found yet !
    }

    void FunctionGraph::reset()
    {
        current = start;
        previous = exit;
        error   = false;
    }

    bool FunctionGraph::isFinished() const 
    {
        return current == exit;
    }

    bool FunctionGraph::inError() const 
    {
        return error;
    }

    const std::string& FunctionGraph::getName() const
    {
        return myName;
    }

    void FunctionGraph::setName(const std::string& name)
    {
        myName = name;
    }

    std::string FunctionGraph::getText() const
    {
        return _text;
    }

    void FunctionGraph::setText(const std::string& text)
    {
        _text = text;
    }

    int FunctionGraph::getLineNumber() const
    {
        return get(vertex_command, program)[current].getLineNumber();
    }

    FunctionGraph* FunctionGraph::copy( std::map<const DataSourceBase*, DataSourceBase*>& replacementdss ) const
    {
        typedef boost::property_map<Graph, vertex_index_t>::const_type indexmap_t;
        typedef boost::graph_traits<Graph>::vertex_descriptor vd_t;
        typedef std::vector<vd_t> o2cvect_t;
        typedef boost::iterator_property_map<o2cvect_t::iterator, indexmap_t, vd_t, vd_t&> o2cmap_t;
        FunctionGraph* ret = new FunctionGraph( getName() );
        indexmap_t indexmap = get( vertex_index, program );
        // here we assume that the indexing of program is set properly...
        o2cvect_t o2cvect( num_vertices( program ) );
        o2cmap_t o2cmap( o2cvect.begin(), indexmap );

        // The replacementdss map contains mappings from this->datasource to copy->datasource, 
        // thus we can rebuild a vector<TaskAttributeBase*>, which will be automagically be
        // found by copy_graph.
        for (unsigned int i=0; i < args.size(); ++i)
            ret->addArgument( args[i]->copy( replacementdss ) );

        boost::copy_graph( program, ret->program,
                           boost::vertex_copy( GraphVertexCopier( program, ret->program, replacementdss ) ).
                           edge_copy( GraphEdgeCopier( program, ret->program, replacementdss ) ).
                           orig_to_copy( o2cmap ) );

        ret->start = o2cmap[start];
        ret->exit = o2cmap[exit];
        ret->current = o2cmap[current];
        ret->previous = o2cmap[previous];

        return ret;
    }

    FunctionGraph* FunctionGraph::clone() const
    {
        return new FunctionGraph(*this);
    }

    void FunctionGraph::debugPrintout() const {
        graph_traits<Graph>::vertex_iterator v,vend;
        tie(v,vend) = vertices(program);
        boost::property_map<Graph, vertex_command_t>::const_type
            cmap = get(vertex_command, program);
        boost::property_map<Graph, vertex_index_t>::const_type
            imap = get(vertex_index, program);
        std::cerr << "program " << getName() << std::endl;
        std::cerr << " number of vertices: " << boost::num_vertices(program) << std::endl;
        for ( ; v != vend; ++v )
        {
            int index = get( imap, *v );
            CommandInterface* cmd = get( cmap, *v ).getCommand();
            if ( cmd )
                std::cerr << " " << index << " " << typeid( *cmd ).name() << std::endl;
            else
                std::cerr << " " << index << " (null)" << std::endl;
        }
    }
}

