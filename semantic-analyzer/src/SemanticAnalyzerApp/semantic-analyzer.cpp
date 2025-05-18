#include <Parser/Common.hpp>
#include <Sema/InheritanceGraphBuilder.hpp>
#include <Sema/SemanticChecker.hpp>

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++) {
        bool ok = parser::Parse(argv[i]);
        if (!ok) {
            return EXIT_FAILURE;
        }
    }

    auto program = parser::GetProgram();

    sema::InheritanceGraphBuilder IGB;
    IGB.Visit(program);

    auto classes = IGB.GetTopoSortedClasses();
    auto class_map = IGB.GetClassMap();
    auto inh_graph = IGB.GetInheritanceGraph();

    sema::SemanticChecker SC(std::move(classes), std::move(class_map), std::move(inh_graph));
    SC.Visit(program);

    // program->DumpWithTypes(std::cout, 0);

    return 0;
}
