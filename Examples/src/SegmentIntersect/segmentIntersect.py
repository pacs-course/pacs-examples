import sys

sys.path.insert(0, '.')  # Adjust path if needed

try:
    from pyIntersect import segment_intersect
    from pyIntersect import Point, EdgeGeo, IntersectionStatus
except ImportError:
    print("Error: Could not import pyIntersect module. Make sure it's compiled and in the path.")
    sys.exit(1)


def get_segment_from_user(segment_number):
    """Get segment coordinates from user input."""
    print(f"\nEnter coordinates for segment {segment_number}:")
    try:
        x1 = float(input(f"  x1: "))
        y1 = float(input(f"  y1: "))
        x2 = float(input(f"  x2: "))
        y2 = float(input(f"  y2: "))
        a = Point(x1, y1)
        b = Point(x2, y2)
        EdgeGeo_Point = EdgeGeo(p1=a, p2=b)
        return EdgeGeo_Point
    except ValueError:
        print("Error: Please enter valid numbers.")
        return None


def main():
    print("Segment Intersection Test")
    print("=" * 40)
    
    # Get first segment
    segment1 = get_segment_from_user(1)
    if segment1 is None:
        return
    
    # Get second segment
    segment2 = get_segment_from_user(2)
    if segment2 is None:
        return
    
    # Find intersection
    result = segment_intersect(segment1, segment2)
    
    # Display results
    print("\n" + "=" * 40)
    print("Result:")
    print(result)
    print("=" * 40)


if __name__ == "__main__":
    main()