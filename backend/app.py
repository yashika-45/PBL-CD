from flask import Flask, request, jsonify
from phase2.simplify_ast import simplify_ast
import subprocess
import os
import traceback
from flask_cors import CORS
from tokens_map import TOKEN_MAP  # Your token mapping file
import json

app = Flask(__name__)
CORS(app)  # Enable CORS for frontend communication

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


def parse_lexical_output(raw_output):
    lines = raw_output.splitlines()
    result = []
    for line in lines:
        if ":" not in line or "Symbol table" in line or "Constants Table" in line:
            continue
        parts = line.strip().split(":")
        if len(parts) == 2:
            lexeme = parts[0].strip()
            try:
                code = int(parts[1].strip())
                token_name = TOKEN_MAP.get(code, "UNKNOWN")
                result.append({"lexeme": lexeme, "code": code, "tokenName": token_name})
            except ValueError:
                continue
    return result

@app.route('/analyze', methods=['POST'])
def analyze():
    try:
        analysis_type = request.form.get('type')
        print("Received analysis type:", analysis_type)

        if 'file' in request.files:
            file = request.files['file']
            code = file.read().decode()
        else:
            code = request.form.get('code')

        print("Received code:\n", code)

        if not code:
            print("❌ No code provided in request")
            return jsonify({'error': 'No code provided'}), 400

        input_path = os.path.join(BASE_DIR, 'temp', 'input.c')
        os.makedirs(os.path.dirname(input_path), exist_ok=True)
        with open(input_path, 'w') as f:
            f.write(code)
        print(f"✅ Code written to: {input_path}")

        if analysis_type == 'lexical':
            cmd = 'analyze.bat'
            working_dir = os.path.join(BASE_DIR, 'Project-1')
        elif analysis_type == 'syntax':
            cmd = 'phase2.bat'
            working_dir = os.path.join(BASE_DIR, 'phase2')
        elif analysis_type == 'semantic':
            cmd = 'semantic.bat'
            working_dir = os.path.join(BASE_DIR, 'phase3')
        else:
            print("❌ Invalid analysis type")
            return jsonify({'error': 'Invalid analysis type'}), 400

        print(f"🛠 Running command: {cmd} in {working_dir}")
        raw_output = subprocess.check_output(cmd, shell=True, stderr=subprocess.STDOUT, cwd=working_dir).decode()
        print("📄 Subprocess output:\n", raw_output)

        if analysis_type == 'lexical':
            parsed_output = parse_lexical_output(raw_output)
            return jsonify({'output': parsed_output})

        elif analysis_type == 'syntax':
            json_path = os.path.join(working_dir, 'tree.json')
            error_path = os.path.join(working_dir, 'syntax_errors.txt')

            syntax_errors = []
            if os.path.exists(error_path):
                with open(error_path, 'r') as f:
                    syntax_errors = f.read().splitlines()

            if os.path.exists(json_path):
                with open(json_path, 'r') as f:
                    full_ast = json.load(f)
                minimal_ast = simplify_ast(full_ast)
                return jsonify({
                    'output': raw_output,
                    'ast': full_ast,
                    'minimal_ast': minimal_ast,
                    'errors': syntax_errors  # Send syntax errors to UI
                })
            else:
                return jsonify({
                    'output': raw_output,
                    'error': 'Parse tree not generated',
                    'errors': syntax_errors
                })

        elif analysis_type == 'semantic':
            error_path = os.path.join(working_dir, 'error.txt')
            apt_path = os.path.join(working_dir, 'apt.json')

            errors = []
            if os.path.exists(error_path):
                with open(error_path, 'r') as f:
                    errors = f.read().splitlines()

            apt = {}
            if os.path.exists(apt_path):
                with open(apt_path, 'r') as f:
                    apt = json.load(f)

            return jsonify({
                'output': raw_output,
                'errors': errors,
                'apt': apt
            })

    except subprocess.CalledProcessError as e:
        print("🚨 Script execution error:\n", e.output.decode())
        return jsonify({'error': e.output.decode()}), 500
    except Exception as ex:
        print("🔥 Unexpected server error:")
        traceback.print_exc()
        return jsonify({'error': str(ex)}), 500


if __name__ == '__main__':
    app.run(debug=True)