//
//  ContentView.swift
//  Number to Words
//
//  Created by Levi McFall on 5/12/20.
//  Copyright © 2020 Levi McFall. All rights reserved.
//

import SwiftUI

func splitForInt(_ string: String) -> [String] {
       var input = string
       var result: [String] = []
       while input.count > 19 {
           let some19 = String(input.prefix(19))
           result += [some19]
           input.removeSubrange(input.startIndex...input.index(input.startIndex, offsetBy: 19 - 1))
       }
       result += [input]
       return result
   }

extension String {
    mutating func removeInPlace(allOf element: String.Element) {
        while self.contains(element) {
            self.remove(at: self.firstIndex(of: element)!)
        }
    }
    func hasValidChs() -> Bool {
        let validSymbols: [String.Element] = [" ", ",", ".", "-", "^"] //, "*"]
        var input = self
        for symbol in validSymbols {
            if !input.contains(symbol) { continue }
            if symbol == "," { input.removeInPlace(allOf: ","); continue}
            if symbol == " " { input.removeInPlace(allOf: " "); continue}
            input.remove(at: input.firstIndex(of: symbol)!)
        }
        let intChunks = splitForInt(input)
        for chunk in intChunks { if Int(chunk) == nil { return false } }
        return true
    }
    func clearFirstIndexes(number: Int) -> String {
        let startIndex = self.index(self.startIndex, offsetBy: number)
        let endIndex = self.endIndex
        return "\(self[startIndex..<endIndex])"
    }
    func clearLastIndexes(number: Int) -> String {
        let endIndex = self.index(self.endIndex, offsetBy: -number)
        return "\(self[..<endIndex])"
    }
    func firstIndexes(number: Int) -> String {
        let endIndex = self.index(self.startIndex, offsetBy: number)
        return "\(self[..<endIndex])"
    }

}
extension Double {
    var asRenderedString: String? {
        var stringInput = String(self)
        if !stringInput.contains("e+") { return nil }
        stringInput.remove(at: stringInput.firstIndex(of: "e")!)
        let baseAndE = stringInput.split(separator: "+")
        if baseAndE.count != 2 { return nil }
        var base = String(baseAndE[0])
        guard let e = Int(baseAndE[1]) else { return nil }
        
        let originalDecimalIndex: String.Index? = base.firstIndex(of: ".")
        
        if originalDecimalIndex == nil {
           var result = base
            for _ in 1...e {
                result += "0"
            }
            return result
        }
        
        var newDecimalIndex = originalDecimalIndex!
        base.remove(at: newDecimalIndex)
        var result = base
        
        for _ in 1...e {
            if newDecimalIndex != result.endIndex {
                newDecimalIndex = result.index(after: newDecimalIndex)
                continue
            }
            result += "0"
            newDecimalIndex = result.index(after: newDecimalIndex)
        }
        
        if result.endIndex != newDecimalIndex { result.insert(".", at: newDecimalIndex) }
        
        return result
    }
}



struct ContentView: View {
    
    
    @State private var input = ""
    
    let onesDict = [
        "1": "one",
        "2": "two",
        "3": "three",
        "4": "four",
        "5": "five",
        "6": "six",
        "7": "seven",
        "8": "eight",
        "9": "nine"
    ]
    let teensDict = [
        "10": "ten",
        "11": "eleven",
        "12": "twelve",
        "13": "thirteen",
        "14": "fourteen",
        "15": "fifteen",
        "16": "sixteen",
        "17": "seventeen",
        "18": "eighteen",
        "19": "nineteen"
    ]


    let tensDict = [
        "10": "ten",
        "20": "twenty",
        "30": "thrity",
        "40": "forty",
        "50": "fifty",
        "60": "sixty",
        "70": "seventy",
        "80": "eighty",
        "90": "ninety"
    ]

    let bigChunkNames = ["thousand", "million", "billion", "trillion", "quadrillion", "quintillion", "sextillion", "septillion", "octillion", "nonillion", "decillion", "undecillion", "doudecillion", "tredecillion", "quattordecilion", "quindecillion", "sedecillion", "septendecillion", "octodecillion", "novendecillion", "vigintillion", "unvigintillion", "douvigintillion", "tresvigintillion", "quattorvigintillion", "quinvigintillion", "sesvigintillion", "septemvigintillion", "octovigintillion", "novemvigintillion", "trigintillion", "untrigintillion", "doutrigintillion", "trestrigintillion", "quattortrigintillion", "quintrigintillion", "sestrigintillion", "septentrigintillion", "octotrigintillion", "noventrigintillion", "quadragintillion", "unquadragintillion", "douquadragintillion", "tresquadragintillion", "quattorquadragintillion", "quinquadragintillion", "sesquadragintillion", "septenquadragintillion", "octoquadragintillion", "novenquadragintillion"]



    func addChunkNames(to input: [String]) -> [String] {
        var result = [String]()
        var chunkNameIndex = input.count - 2 // 2 - one for hundreds chunk before thousands and one for index
        for translation in input {
            if chunkNameIndex < 0 || translation == "" { result.append("\(translation)"); continue}
            result.append("\(translation) \(bigChunkNames[chunkNameIndex])")
            chunkNameIndex -= 1
        }
        return result
    }

    // Splittin'
   

    func splitIntoThrees(in input: String, count: Int) -> [String] {
        var start = 0
        var end = 2
        var result = [String]()
        for _ in 1...count {
            let startIndex = input.index(input.startIndex, offsetBy: start)
            let cutIndex = input.index(input.startIndex, offsetBy: end)
            result += ["\(input[startIndex...cutIndex])"]
            start += 3
            end += 3
            if end >= count { break }
        }
        return result
    }

    // Transaltions
    func trans2 (_ input: String) -> String {
        if let teen = teensDict[input] {
                   return teen
               } else {
                   let tensPart = tensDict["\(input.first!)0"] ?? ""
                   let onesPart = onesDict["\(input.last!)"] ?? ""
                   if tensPart != "" && onesPart != "" { return "\(tensPart)-\(onesPart)"}
                   else if tensPart == "" { return onesPart }
                   else if onesPart == "" { return tensPart }
                   else { return "wrong" }
               }
    }

    func transUpTo3 (_ input: String) -> String {
    //    if input == "000" { return "zeros"}
        let count = input.count
        
        switch count {
            
        case 1:
            return onesDict[input] ?? ""
            
        case 2:
           return trans2(input)
            
        case 3:
            let hundredsPart = onesDict["\(input.first!)"] ?? ""
            
            let tensElementIndex = input.index(after: input.startIndex)
            let tensSearch = "\(input[tensElementIndex])"
            let onesSearch = "\(input.last!)"
            
            let tensAndOnes = trans2("\(tensSearch + onesSearch)")
            var tensAndOnesSpace = " "
            if tensAndOnes == "" { tensAndOnesSpace = "" }
            
            if hundredsPart != "" { return "\(hundredsPart) hundred\(tensAndOnesSpace)\(tensAndOnes)"}
            else { return tensAndOnes }
            
        default:
            return "wrong"
        }
    }

    func handlePower(_ input: String) -> String? {
        let numbers = input.split(separator: "^")
        if numbers.count != 2 { return nil }
        guard let base = Double(numbers[0]) else { return nil }
        guard let exponent = Double(numbers[1]) else { return nil }
        let power = pow(base, exponent)
        
        var translated = ""
        if String(power).contains("e") {
            guard let rendered = power.asRenderedString else { return nil }
            translated = translate("\(rendered)")
        } else { translated = translate("\(String(power))")}
        
        if translated.hasSuffix(" and zero tenths") { translated.removeSubrange(translated.index(translated.endIndex, offsetBy: -16)...)}
        return "\(translated)"
    }

    func handleDecimal(input: String) -> String {
        var wOPeriod = input
        wOPeriod.removeFirst()
        
        let count = wOPeriod.count
        let trans = handleInt(wOPeriod)
        
        var zeros = ""
        for _ in 0..<count {
            zeros += "0"
        }
        
        var denominator = handleInt("1\(zeros)")
        if denominator.hasPrefix("one ") { denominator.removeSubrange(denominator.startIndex...denominator.firstIndex(of: " ")!)}
        
       return "and \(trans) \(denominator)ths"
    }

    func handleInt(_ int: String) -> String {
        if int == "" { return "" }
        var result = ""
        var input = int
        
        let count = input.count
        
        if input == "0" { return "zero" }
        
        // Handle inputs with non-optimal length
        if count < 3 {
            if count == 1 { return "\(result)\(onesDict[input] ?? "Invalid Input")" }
            return "\(result)\(trans2(input))"
        }
        if count > 153 { return "dang - that's a big number... too big." }
       
        // Remove 0's at start
        while input.first == "0" { input.removeFirst() }
        
        
        // Get chunks
        let remainder = count % 3
        var chunks = [String]()
        
        switch remainder {
        case 0:
            chunks += splitIntoThrees(in: input, count: count)
        case 1:
            chunks += ["\(input.first!)"]
            let rounded = input.clearFirstIndexes(number: 1)
            chunks += splitIntoThrees(in: rounded, count: count - remainder)
        case 2:
            chunks += ["\(input.firstIndexes(number: 2))"]
            let rounded = input.clearFirstIndexes(number: 2)
            chunks += splitIntoThrees(in: rounded, count: count - remainder)
       default:
              print("wrong")
        }
        
        // Translate Each Chunk
        var translations = [String]()
        for chunk in chunks {
            let translation = transUpTo3(chunk)
            translations.append(translation)
        }
        
        // Add CHunk Names
        let withChunkNames = addChunkNames(to: translations)
        for chunk in withChunkNames {
            if chunk != "" && chunk.first != "z" { result.append(contentsOf: "\(chunk), ") }
        }
        
        // Finalize Result
        result = result.clearLastIndexes(number: 2)
        return result
    }



    func translate(_ string: String) -> String {
        
        var input = string
        
        input.removeInPlace(allOf: ",")
        input.removeInPlace(allOf: " ")
        
        if input == "" { return "" }
        if !input.hasValidChs() { return "Invalid Input" }
        
        //account for negative
        var negativePrefix = ""
        if input.hasPrefix("-") { input.removeFirst(); negativePrefix = "negative " }
        
        // If in Scientific notation, do that, or stop invalid input with * and ^
        //    if input.contains("*") {
        //        if !Bool(input.contains("*") && input.contains("^") && input.last != "^")  { return "Incomplete Scientific Notaition" }
        //        return handleSci(input) ?? "Invalid Scientific Notation"
        //    }
        
        // Expponents
        if input.contains("^") {
            guard let power = handlePower(input) else { return "Incomplete Exponent" }
            return "\(negativePrefix)\(power)"
        }
        
        
        // See if it is a decimals
        if input.contains(".") {
            
            // Return Int if there is Decimal Point without decimal
            if input.hasSuffix(".") {
                var chunk = input
                chunk.removeLast()
                return "\(negativePrefix)\(handleInt(String(chunk)))"
            }
            
            // Regular Decimal -> translate the int, traslate the decimal, and put them together
            let chunks = input.split(separator: ".")
            let int = handleInt(String(chunks[0]))
            let decimal = handleDecimal(input: String(".\(chunks[1])"))
            
            return "\(negativePrefix)\(int) \(decimal)"
            
        } else {
            return "\(negativePrefix)\(handleInt(input))"
        }
    }

    
    
    var result: String? {
        return translate(input)
    }
    
    
    var body: some View {
        NavigationView {
            Form {
                TextField("Enter Your Number", text: $input)
                    .keyboardType(.numbersAndPunctuation)
                Section(header: Text("Words:")) {
                    Text("\(result ?? "")")
                }
            }
            .navigationBarTitle("Number to Words")
        }
       
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
