package base

import (
	"os"
	"path/filepath"
	"sort"
	"strings"
	"unicode"
)

// gerado pelo Claude
// FuzzyFindPlugin busca plugins instalados em NvPath.Plugins
// que correspondam ao pluginName de forma fuzzy.
// Retorna uma lista de nomes de plugins ordenada por relevância.
func FuzzyFindPlugin(pluginName string) []string {
	entries, err := os.ReadDir(NvPath.PluginsPath)
	if err != nil {
		return nil
	}

	query := normalize(pluginName)

	type candidate struct {
		name  string
		score int
	}

	var candidates []candidate

	for _, entry := range entries {
		if entry.IsDir() {
			continue
		}

		fileName := entry.Name()
		// Remove extensão .lua
		plugName := strings.TrimSuffix(fileName, filepath.Ext(fileName))
		normalized := normalize(plugName)

		score := computeScore(query, normalized)
		if score > 0 {
			candidates = append(candidates, candidate{name: plugName, score: score})
		}
	}

	// Ordena do maior score para o menor
	sort.Slice(candidates, func(i, j int) bool {
		return candidates[i].score > candidates[j].score
	})

	results := make([]string, 0, len(candidates))
	for _, c := range candidates {
		results = append(results, c.name)
	}

	return results
}

// normalize transforma string em lowercase e remove separadores comuns

func normalize(s string) string {
	s = strings.ToLower(s)
	s = strings.Map(func(r rune) rune {
		if r == '-' || r == '_' || r == '.' {
			return ' '
		}
		return r
	}, s)
	return strings.TrimSpace(s)
}

// computeScore calcula a relevância entre query e target.
// Quanto maior, mais relevante. 0 significa sem match.
func computeScore(query, target string) int {
	if query == "" {
		return 1
	}

	// Match exato
	if query == target {
		return 1000
	}

	// Target contém query como substring
	if strings.Contains(target, query) {
		return 800 - len(target) // prefere nomes mais curtos
	}

	// Query é prefixo do target
	if strings.HasPrefix(target, query) {
		return 700
	}

	// Todas as palavras da query aparecem no target
	queryWords := strings.Fields(query)
	targetWords := strings.Fields(target)
	if allWordsMatch(queryWords, targetWords) {
		return 600
	}

	// Subsequência: todos os caracteres da query aparecem em ordem no target
	if isSubsequence(query, target) {
		// Score baseado em quão "compacta" é a subsequência
		return 400 - levenshtein(query, target)
	}

	// Distância de edição baixa (tolerância a typos)
	dist := levenshtein(query, target)
	maxDist := max(len([]rune(query)), len([]rune(target))) / 3
	if dist <= maxDist && maxDist > 0 {
		return 200 - dist
	}

	// Iniciais da query batem com iniciais das palavras do target
	if initialsMatch(query, targetWords) {
		return 100
	}

	return 0
}

// allWordsMatch verifica se todas as palavras da query existem no target
func allWordsMatch(queryWords, targetWords []string) bool {
	for _, qw := range queryWords {
		found := false
		for _, tw := range targetWords {
			if strings.Contains(tw, qw) {
				found = true
				break
			}
		}
		if !found {
			return false
		}
	}
	return true
}

// isSubsequence verifica se query é subsequência de target
func isSubsequence(query, target string) bool {
	qi := 0
	qRunes := []rune(query)
	for _, r := range target {
		if qi < len(qRunes) && unicode.ToLower(r) == unicode.ToLower(qRunes[qi]) {
			qi++
		}
	}
	return qi == len(qRunes)
}

// initialsMatch verifica se os caracteres da query batem com as iniciais das palavras do target
func initialsMatch(query string, targetWords []string) bool {
	initials := ""
	for _, w := range targetWords {
		if len(w) > 0 {
			initials += string([]rune(w)[0])
		}
	}
	return isSubsequence(query, initials)
}

// levenshtein calcula a distância de edição entre duas strings
func levenshtein(a, b string) int {
	ar, br := []rune(a), []rune(b)
	la, lb := len(ar), len(br)

	dp := make([][]int, la+1)
	for i := range dp {
		dp[i] = make([]int, lb+1)
		dp[i][0] = i
	}
	for j := range dp[0] {
		dp[0][j] = j
	}

	for i := 1; i <= la; i++ {
		for j := 1; j <= lb; j++ {
			if ar[i-1] == br[j-1] {
				dp[i][j] = dp[i-1][j-1]
			} else {
				dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])
			}
		}
	}

	return dp[la][lb]
}

func min(a, b, c int) int {
	if a < b {
		if a < c {
			return a
		}
		return c
	}
	if b < c {
		return b
	}
	return c
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}
