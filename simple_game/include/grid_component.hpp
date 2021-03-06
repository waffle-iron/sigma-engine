#ifndef GRID_COMPONENT_HPP
#define GRID_COMPONENT_HPP

struct grid_component {
    int rows;
    int columns;
    float row_spacing;
    float column_spacing;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& rows;
        ar& columns;
        ar& row_spacing;
        ar& column_spacing;
    }
};

#endif // GRID_COMPONENT_HPP
