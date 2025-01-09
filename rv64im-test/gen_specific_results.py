import re
import os

def parse_results(file_path):
    """
    Parses the results from the given file and returns a dictionary.
    Each key is a result name, and its value is another dictionary of fields and their values.
    """
    results = {}
    current_result = None

    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()
            if not line:
                continue  # Skip empty lines

            # Check if the line signifies a new result block
            match_result = re.match(r'(\w+)_result:', line)
            if match_result:
                current_result = match_result.group(1)
                results[current_result] = {}
                continue

            # If within a result block, parse the field and value
            if current_result:
                match_field = re.match(r'([^:]+):\s*(.+)', line)
                if match_field:
                    field = match_field.group(1).strip()
                    value = match_field.group(2).strip()
                    results[current_result][field] = value

    return results

def display_menu(fields):
    """
    Displays the menu of fields with assigned numbers.
    """
    print("\nAvailable Fields:")
    for idx, field in enumerate(fields, start=1):
        print(f"{idx}. {field}")

def get_user_choice(num_fields):
    """
    Prompts the user to enter a number corresponding to a field.
    """
    while True:
        try:
            choice = int(input(f"\nEnter the number of the field you want to extract (1-{num_fields}): "))
            if 1 <= choice <= num_fields:
                return choice
            else:
                print(f"Please enter a number between 1 and {num_fields}.")
        except ValueError:
            print("Invalid input. Please enter a valid number.")

def generate_output_file(field, results, output_dir="results_summary"):
    """
    Generates an output file for the specified field containing data from all results.
    """
    # Ensure the output directory exists
    os.makedirs(output_dir, exist_ok=True)

    # Create a valid filename by replacing spaces with underscores and removing problematic characters
    filename = f"{field.replace(' ', '_')}.txt"
    file_path = os.path.join(output_dir, filename)

    with open(file_path, 'w') as outfile:
        outfile.write(f"----- {field} Result -----\n")
        for result_name, fields in results.items():
            # Remove '_result' suffix if present
            clean_name = re.sub(r'_result$', '', result_name)
            value = fields.get(field, 'N/A')
            outfile.write(f"{clean_name}: {value}\n")

    print(f"\nData for '{field}' has been written to '{file_path}'.")

def main():
    # Path to the input data file
    input_file = 'results_summary/results.txt'

    # Check if the input file exists
    if not os.path.isfile(input_file):
        print(f"Error: The file '{input_file}' does not exist in the current directory.")
        return

    # Parse the results
    results = parse_results(input_file)

    if not results:
        print("No results found in the input file.")
        return

    # Collect all unique fields
    all_fields = set()
    for fields in results.values():
        all_fields.update(fields.keys())

    # Sort the fields for consistent ordering
    sorted_fields = sorted(all_fields)

    # Display the menu
    display_menu(sorted_fields)

    # Get user choice
    choice = get_user_choice(len(sorted_fields))
    selected_field = sorted_fields[choice - 1]

    # Generate the output file
    generate_output_file(selected_field, results)

if __name__ == "__main__":
    main()
