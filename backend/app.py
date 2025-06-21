from flask import Flask, request, jsonify
import subprocess
import os
import traceback
from flask_cors import CORS
from tokens_map import TOKEN_MAP  # Your token mapping file

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

        # Support textarea code or file
        if 'file' in request.files:
            file = request.files['file']
            code = file.read().decode()
        else:
            code = request.form.get('code')

        print("Received code:\n", code)

        if not code:
            print("❌ No code provided in request")
            return jsonify({'error': 'No code provided'}), 400

        # Save code to input.c
        input_path = os.path.join(BASE_DIR, 'temp', 'input.c')
        os.makedirs(os.path.dirname(input_path), exist_ok=True)
        with open(input_path, 'w') as f:
            f.write(code)
        print(f"✅ Code written to: {input_path}")

        # Select batch script and working directory
        if analysis_type == 'lexical':
            cmd = 'analyze.bat'
            working_dir = os.path.join(BASE_DIR, 'Project-1')
        elif analysis_type == 'syntax':
            cmd = 'phase2.bat'
            working_dir = os.path.join(BASE_DIR, 'phase2')
        else:
            print("❌ Invalid analysis type")
            return jsonify({'error': 'Invalid analysis type'}), 400

        # Execute batch script
        print(f"🛠 Running command: {cmd} in {working_dir}")
        raw_output = subprocess.check_output(cmd, shell=True, stderr=subprocess.STDOUT, cwd=working_dir).decode()
        print("📄 Subprocess output:\n", raw_output)

        if analysis_type == 'lexical':
            parsed_output = parse_lexical_output(raw_output)
            print("✅ Lexical analysis completed")
            return jsonify({'output': parsed_output})

        # For syntax analysis, try reading AST
        json_path = os.path.join(working_dir, 'tree.json')
        print(f"🔍 Looking for AST at: {json_path}")
        if os.path.exists(json_path):
            with open(json_path, 'r') as f:
                json_data = f.read()
            print("✅ AST successfully loaded")
            return jsonify({
                'output': raw_output,
                'ast': json_data
            })
        else:
            print("❌ AST not found: tree.json missing")
            return jsonify({
                'output': raw_output,
                'error': 'Parse tree not generated'
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
