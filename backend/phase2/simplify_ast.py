# def simplify_ast(node):
#     def simplify(node):
#         if not isinstance(node, dict) or 'label' not in node:
#             return None

#         label = node['label']

#         label_map = {
#             "Assignment": "=",
#             "Addition": "+",
#             "Subtraction": "-",
#             "Multiplication": "*",
#             "Division": "/"
#         }

#         if label in label_map:
#             return {
#                 "label": label_map[label],
#                 "children": [simplify(c) for c in node.get("children", []) if simplify(c)]
#             }

#         elif label in {"Identifier", "Number"}:
#             if node.get("children"):
#                 return { "label": node["children"][0]["label"] }

#         elif label == "FunctionCall":
#             name = node["children"][0]["label"]
#             args = [simplify(c) for c in node["children"][1:] if simplify(c)]
#             return { "label": name, "children": args }

#         # Recursively simplify all children and collect relevant subtrees
#         results = []
#         for child in node.get("children", []):
#             res = simplify(child)
#             if res:
#                 results.append(res)

#         if results:
#             return results if len(results) > 1 else results[0]

#         return None

#     simplified = simplify(node)

#     # Ensure output is a flat list of top-level ASTs
#     if isinstance(simplified, list):
#         return { "label": "Root", "children": simplified }
#     elif isinstance(simplified, dict):
#         return { "label": "Root", "children": [simplified] }
#     else:
#         return { "label": "Root", "children": [] }


#for for loop this one

# def simplify_ast(node):
#     def simplify(node):
#         if not isinstance(node, dict) or 'label' not in node:
#             return None

#         label_map = {
#             "Assignment": "=",
#             "Addition": "+",
#             "Subtraction": "-",
#             "Multiplication": "*",
#             "Division": "/",
#             "Equality": "==",
#             "Inequality": "!=",
#             "LessThan": "<",
#             "GreaterThan": ">",
#             "LessThanOrEqual": "<=",
#             "GreaterThanOrEqual": ">="
#         }

#         label = node["label"]

#         if label in label_map:
#             children = [simplify(child) for child in node.get("children", [])]
#             return {
#                 "label": label_map[label],
#                 "children": [child for child in children if child]
#             }

#         elif label == "FunctionCall":
#             fn_name = node["children"][0]["label"]
#             args = [simplify(c) for c in node["children"][1:] if simplify(c)]
#             return {
#                 "label": fn_name,
#                 "children": args
#             }

#         elif label in {"Identifier", "Number"}:
#             if node.get("children"):
#                 return { "label": node["children"][0]["label"] }
#             return None

#         # Otherwise, go deeper and collect relevant children
#         results = []
#         for child in node.get("children", []):
#             result = simplify(child)
#             if result:
#                 if isinstance(result, list):
#                     results.extend(result)
#                 else:
#                     results.append(result)

#         return results

#     result = simplify(node)

#     # Wrap collected results properly under a "Root" node
#     if isinstance(result, dict):
#         return { "label": "Root", "children": [result] }
#     elif isinstance(result, list):
#         return { "label": "Root", "children": result }
#     else:
#         return { "label": "Root", "children": [] }

def simplify_ast(node):
    def simplify(node):
        if not isinstance(node, dict) or 'label' not in node:
            return None

        label_map = {
            "Assignment": "=",
            "Addition": "+",
            "Subtraction": "-",
            "Multiplication": "*",
            "Division": "/",
            "Equality": "==",
            "Inequality": "!=",
            "LessThan": "<",
            "GreaterThan": ">",
            "LessThanOrEqual": "<=",
            "GreaterThanOrEqual": ">="
        }

        label = node["label"]

        if label in label_map:
            children = [simplify(child) for child in node.get("children", [])]
            return {
                "label": label_map[label],
                "children": [child for child in children if child]
            }

        elif label == "FunctionCall":
            fn_name = node["children"][0]["label"]
            args = []

            if len(node["children"]) > 1:
                arg_node = node["children"][1]
                if arg_node["label"] == "ArgumentList":
                    for child in arg_node.get("children", []):
                        simplified = simplify(child)
                        if simplified:
                            args.append(simplified)
                else:
                    simplified = simplify(arg_node)
                    if simplified:
                        args.append(simplified)

            return {
                "label": fn_name,
                "children": args
            }

        elif label in {"Identifier", "Number"}:
            if node.get("children"):
                return { "label": node["children"][0]["label"] }
            return None

        # Otherwise, go deeper and collect relevant children
        results = []
        for child in node.get("children", []):
            result = simplify(child)
            if result:
                if isinstance(result, list):
                    results.extend(result)
                else:
                    results.append(result)

        return results

    result = simplify(node)

    # Wrap collected results properly under a "Root" node
    if isinstance(result, dict):
        return { "label": "Root", "children": [result] }
    elif isinstance(result, list):
        return { "label": "Root", "children": result }
    else:
        return { "label": "Root", "children": [] }
