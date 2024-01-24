def main():
    
    def partner(rank,phrase):
        v = 2**phrase
        return rank ^ v
        
    for rank in range(8):
        partners = [partner(rank,phrase) for phrase in range(3)]
        print(f'P_{rank} -> {partners}')

if __name__ == '__main__':
    main()