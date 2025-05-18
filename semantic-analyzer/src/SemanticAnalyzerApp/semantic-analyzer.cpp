#include <Parser/Common.hpp>
#include <Sema/InheritanceGraphBuilder.hpp>
#include <Sema/SemanticChecker.hpp>

int main(int argc, char** argv)
{
    Program* program = new Program();
    for (int i = 1; i < argc; i++) {
        bool ok = parser::Parse(argv[i]);
        if (!ok) {
            return EXIT_FAILURE;
        }

        if (i == 1) {
            program = parser::GetProgram();
        } else {
            auto cur_classes = program->GetClasses();
            for (auto&& class_ : *parser::GetProgram()->GetClasses()) {
                cur_classes->PushBack(std::move(class_));
            }
        }
    }

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
