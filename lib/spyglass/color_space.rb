module Spyglass
  module ColorSpace
    def self.[](mapping)
      code = mapping.to_a.join("_TO_").upcase
      if !const_defined?(code)
        raise ArgumentError.new("color conversion not available")
      end

      const_get(code.upcase)
    end
  end
end
