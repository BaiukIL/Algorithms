def write_operator(out, symb):
    out.write(f"\
// =================================== {symb} \
=======================================\n");

    out.write("\
    Point2D& operator{}=(const Point2D& other) {{\n\
      x_ {}= other.x_;\n\
      y_ {}= other.y_;\n\
      return *this;\n\
    }}\n\n\
".format(symb, symb, symb, symb))

    out.write("\
    Point2D operator{}(const Point2D& other) const {{\n\
      Point2D tmp(*this);\n\
      return tmp {}= other;\n\
    }}\n\n\
".format(symb, symb))

    out.write(f"\
// ================================= {symb} NUM \
=====================================\n");
    out.write("\
    Point2D& operator{}=(Scale_t num) {{\n\
      x_ {}= num;\n\
      y_ {}= num;\n\
      return *this;\n\
    }}\n\n\
".format(symb, symb, symb, symb))

    out.write("\
    Point2D operator{}(Scale_t num) {{\n\
      Point2D tmp(*this);\n\
      return tmp {}= num;\n\
    }}\n\
".format(symb, symb))
    out.write(f"\
//------------------------------------------------------------------------------\
\n\n");


if __name__ == '__main__':
    with open("point_operators.gen.inl", 'w') as out:
        write_operator(out, '+')
        write_operator(out, '-')
        write_operator(out, '*')
        write_operator(out, '/')

