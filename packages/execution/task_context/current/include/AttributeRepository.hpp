#ifndef ATTRIBUTEREPOSITORY_HPP
#define ATTRIBUTEREPOSITORY_HPP

#include <memory>
#include <map>

namespace ORO_Execution
{
    class TaskAttributeBase;

    /**
     * @brief A class for keeping track of values/variables.
     *
     * It stores types in a map,
     * so that they can be referenced to later on.
     */
  class AttributeRepository
  {
    typedef std::map<const std::string, TaskAttributeBase*> map_t;
    map_t values;
  public:
    AttributeRepository();
    ~AttributeRepository();
      /**
       * Erases the whole repository.
       */
    void clear();

    bool isDefined( const std::string& name ) const;

      /**
       * Helper functions to add a constant.
       */
    void addConstant( const std::string& name, bool value );
    void addConstant( const std::string& name, double value );
    void addConstant( const std::string& name, int value );
    void addConstant( const std::string& name, const std::string& value );
      /**
       * The most common way to add or remove a variable to the repository.
       */
    void setValue( const std::string& name, TaskAttributeBase* pc );
    void removeValue( const std::string& name );
    /**
     * Get the value with name name.  If no such value exists, this
     * returns 0.
     */
    TaskAttributeBase* getValue( const std::string& name );
  };
}

#endif
